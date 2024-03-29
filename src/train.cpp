#include "train.h"
#include "track38App.h"

train::train( wxString wxName )
{
    this->name = wxName;
}

void train::createControls( wxPanel* parent )
{
    this->parent = parent;
    this->trainName = new wxStaticText( parent, wxID_ANY, this->name );
    this->speedSlider = new wxSlider( parent, ID_ChangeSpeed, 0, this->maxTrainSpeed*( -1 ), this->maxTrainSpeed, wxDefaultPosition, wxSize( 200, -1 ), wxSL_AUTOTICKS );
    this->stopBtn = new wxButton( parent, ID_StopTrain, wxEmptyString, wxDefaultPosition, wxSize( 35, 35 ) );

    speedSlider->SetValue( 0 );
    this->SetCorrectPNG();
}

void train::SetCorrectPNG()
{
    if ( this->isUp() )
    {
        if ( this->isConnected )
        {
            if ( speedSlider->GetValue() == 0 )
            {
                this->SetButton( disconectPNG );
            }
            else
            {
                this->SetButton( stopPNG );
            }
        }
        else
        {
            this->SetButton( connectPNG );
        }
    }
    else
    {
        this->SetButton( stopPNG );
    }
}

void train::SetButton( wxString file )
{
    wxSize sz = this->stopBtn->GetSize();
    double sf = 0.5;
    sz.Scale( sf, sf );
    wxBitmap bitmap( sz );
    wxImage image = bitmap.ConvertToImage();
    image.LoadFile( wxGetApp().ressourcePath + "icons/" + file, wxBITMAP_TYPE_PNG );
    image.Rescale( sz.GetWidth(), sz.GetHeight(), wxIMAGE_QUALITY_BICUBIC );
    this->stopBtn->SetBitmap( wxBitmap( image ) );
    this->stopBtn->Refresh();
}

void train::OnStop( wxCommandEvent& event ) 
{
    if ( this->isUp() && !bleIsAutorized() )
    {
        wxMessageBox("Blutooth ist not authorized.");
        return;
    }

    if ( this->isUp() && blect && blect->threadTerminated )
    {
        delete blect;
        blect = nullptr;
    }
    
    // try to connect
    if ( ( this->isUp() && !this->isConnected ) && ( speedSlider->GetValue() == 0 ) )
    {
        if ( blect == nullptr )
        {
            //wxMessageBox("try connecting");
            blect = new bleControlThread( this->upHubAdress, this->upHubName );

            blect->setCallbackFunc([&](bool newStatus) {
                wxMutexGuiEnter();

                this->isConnected = newStatus;
                this->SetCorrectPNG();

                wxMutexGuiLeave();

                wxThreadEvent event( wxEVT_THREAD, ID_RefreshBmpButton );
                event.SetInt(-1); // that's all
                wxQueueEvent( this->parent, event.Clone() );
            });

            blect->connect();
        }
    }
    else if ( ( this->isUp() && this->isConnected ) && ( speedSlider->GetValue() == 0 ) )
    {
        if ( this->blect )
            this->blect->RequestTermination();
    }
    else
    {
        this->Stop();
    }
}

void train::Stop()
{
    this->speedSlider->SetValue( 0 );
    this->ChangeSpeed( 0 );
}

void train::OnChangeSpeed( wxCommandEvent& event ) 
{
    this->ChangeSpeed( this->speedSlider->GetValue() );
}

void train::ChangeSpeed( int newSpeed )
{
    this->speed = newSpeed;

    if ( this->isPf() )
    {
        string serialSignal = "<T";

        if ( pfGpio < 10 )
        {
            serialSignal += '0';
        }

        serialSignal += to_string( pfGpio );

        if ( ( pfChannel == 1 ) && ( pfSubChannel == 'R' ) )
            serialSignal += '1';
        else if ( ( pfChannel == 1 ) && ( pfSubChannel == 'B' ) )
            serialSignal += '2';
        else if ( ( pfChannel == 2 ) && ( pfSubChannel == 'R' ) )
            serialSignal += '3';
        else if ( ( pfChannel == 2 ) && ( pfSubChannel == 'B' ) )
            serialSignal += '4';
        else if ( ( pfChannel == 3 ) && ( pfSubChannel == 'R' ) )
            serialSignal += '5';
        else if ( ( pfChannel == 3 ) && ( pfSubChannel == 'B' ) )
            serialSignal += '6';
        else if ( ( pfChannel == 4 ) && ( pfSubChannel == 'R' ) )
            serialSignal += '7';
        else if ( ( pfChannel == 4 ) && ( pfSubChannel == 'B' ) )
            serialSignal += '8';

        switch ( newSpeed )
        {
        case 0:
            serialSignal += 'S';
            break;
        
        //FWD
        case 1:
            serialSignal += 'A';
            break;
        
        case 2:
            serialSignal += 'B';
            break;
        
        case 3:
            serialSignal += 'C';
            break;
        
        case 4:
            serialSignal += 'C';
            break;
        
        case 5:
            serialSignal += 'E';
            break;
        
        case 6:
            serialSignal += 'F';
            break;

        case 7:
            serialSignal += 'G';
            break;
        
        //BWD
        case ( -1 ):
            serialSignal += 'Z';
            break;
        
        case ( -2 ):
            serialSignal += 'Y';
            break;
        
        case ( -3 ):
            serialSignal += 'X';
            break;
        
        case ( -4 ):
            serialSignal += 'W';
            break;
        
        case ( -5 ):
            serialSignal += 'V';
            break;
        
        case ( -6 ):
            serialSignal += 'U';
            break;

        case ( -7 ):
            serialSignal += 'T';
            break;
        
        default:
            break;
        }     

        serialSignal += '>';

        copy( serialSignal.begin(), serialSignal.end(), sendSignal );


        char str_send[ 2 ][ 128 ];
        // unsigned char str_recv[ 128 ];

        strcpy( str_send[ 1 ], sendSignal );
        write_port( this->con, str_send[ 1 ] );
        
        // usleep( 1000000 );
        // int bytes = read_port( con, str_recv, 128 );
		// if( bytes > 0 )
        // {
			// str_recv[ bytes ] = 0;

			// printf( "Received %i bytes from Arduino: '%s'\n", bytes, ( char * )str_recv );
            // wxMessageBox( "OK" );
		// }
    }

    else if ( this->isUp() )
    {
        if ( this->blect )
        {
            int port = portA;
            if ( ( this->upChannel == 'b' ) || ( this->upChannel == 'B' ) )
                port = portB;
            
            // Different behaviour if two motors used; upChannel motor is first.
            int len;
            uint8_t *ary;

            if ( this->upTwoMotorsUsed )
            {
                int port2 = portB;
                if ( port == portB )
                    port2 = portA;

                int len2;
                uint8_t *ary2;

                len = motor_speed( &ary, port, newSpeed * 10 );
                len2 = motor_speed( &ary2, port2, newSpeed * ( -10 ) );

                this->blect->sendCommand( ary, len );
                this->blect->sendCommand( ary2, len2 );
            }
            else
            {
                len = motor_speed( &ary, port, newSpeed * 10 );
                this->blect->sendCommand( ary, len );
            }
        }
    }

    this->SetCorrectPNG();
}

void train::setControl( wxString wxControl )
{
    this->control = string( wxControl.mb_str() );
}

void train::setPort( wxString wxPort )
{
    string stdString = string( wxPort.mb_str() );
    strcpy( port, stdString.c_str() );
    this->portPoint = port;
}

void train::setMaxSpeed( wxString wxSpeed )
{
    this->maxTrainSpeed = wxAtoi( wxSpeed );
}

void train::setHubAdress( wxString wxHub )
{
    this->upHubAdress = string( wxHub.mb_str() );
}

void train::setHubName( wxString wxHub )
{
    this->upHubName = string( wxHub.mb_str() );
}

void train::setUpChannel( wxString wxCh )
{
    this->upChannel = string( wxCh.mb_str() )[0];
}

void train::setTwoMotors( bool is )
{
    this->upTwoMotorsUsed = is;
}

void train::setPfChannel( wxString wxCh )
{
    this->pfChannel = wxAtoi( wxCh );
}

void train::setPfSubChannel( wxString wxSub )
{
    this->pfSubChannel = string( wxSub.mb_str() )[0];
}

void train::setPfGPIO( wxString wxGPIO )
{
    this->pfGpio = wxAtoi( wxGPIO );
}

wxString train::getName()
{
    return name;
}

bool train::isPf()
{
    if ( this->control.compare( "pf" ) == 0 )
       return true;
    return false;    
}

bool train::isUp()
{
    if ( this->control.compare( "up" ) == 0 )
       return true;
    return false;
}

bool train::isRc()
{
    if ( this->control.compare( "rc" ) == 0 )
       return true;
    return false;
}

void train::CloseCon()
{
    if ( this->isPf() )
        close_port( this->con );
    else if ( this->isUp() && this->blect )
        this->blect->RequestTermination();
}

train::~train()
{
    this->CloseCon();
}