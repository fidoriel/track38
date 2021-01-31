#include "traincontrol.h"
#include "track38App.h"

trainControlPanel::trainControlPanel( wxPanel* parent, int id ) : wxScrolledWindow( parent, id )
{
    this->parent = parent;

    //Sizer
    topSizer = new wxFlexGridSizer( 3, 0, 0 );
}

void trainControlPanel::createControlBox()
{
    topSizer->AddSpacer(10);
    topSizer->AddSpacer(10);
    topSizer->AddSpacer(10);
    for (train* & selTrain : this->trains)
    {
        selTrain->createControls( this );
        topSizer->Add( selTrain->trainName, 0, wxALL, 5 );
        topSizer->Add( selTrain->speedSlider, 0, wxALL , 5 );
        topSizer->Add( selTrain->stopBtn, 0, wxALL , 5 );

        selTrain->stopBtn->Bind( wxEVT_BUTTON, &train::OnStop, selTrain );
        selTrain->speedSlider->Bind( wxEVT_SLIDER, &train::OnChangeSpeed, selTrain );
    }

    stopAllBtn = new wxButton( parent, ID_StopAll, "Stop All", wxDefaultPosition, wxDefaultSize );
    stopAllBtn->Bind( wxEVT_BUTTON, &trainControlPanel::OnStopAll, this, ID_StopAll);

    this->Layout();
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();

    this->SetSizer(topSizer);
    this->FitInside();
    this->SetScrollRate(20, 20);
}

void trainControlPanel::loadTrains( std::unordered_map< wxString, int > &cons )
{
    //init Config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "trains.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
    
    track38ConfigTrain->SetPath( "/Train/" );
    trains.clear();

    // Read Train Names
    long idx;
    wxString out;
    bool exists = track38ConfigTrain->GetFirstGroup( out, idx );
    if (  exists == true  )
        trains.push_back( new train(out) );
    
    while ( exists )
    {
        exists = track38ConfigTrain->GetNextGroup( out, idx );
        if (  exists == true  )
            trains.push_back( new train(out) );
    }

    //Fill tain information
    for (train* & selTrain : trains)
    {
        // Access the object through iterator
        track38ConfigTrain->SetPath( "/Train/" );
        track38ConfigTrain->SetPath( selTrain->name );

        selTrain->setControl( track38ConfigTrain->Read( "control", "pf" ) );
        selTrain->setMaxSpeed( track38ConfigTrain->Read( "maxSpeed", "7" ) );
        wxString port = track38ConfigTrain->Read( "port", "" );
        selTrain->setPort( port );
    
        if ( cons.count( port ) )
        {
            // wxMessageBox( "port used" );
            selTrain->con = cons[ port ];
        }
        
        else
        {
            int con = connect_port( selTrain->portPoint );

            if ( con < 0 )
            {
                // usleep( 2000000 );
                con = connect_port( selTrain->portPoint );
                if ( con < 0 )
                {
                    wxString msg;
                    msg.Printf( "The port %s is not avaliable. Please select an other port or plug in the device.", selTrain->port );
                    wxMessageBox( msg, "Port not avaliable");
                }
            }
            
            selTrain->con = con;
            cons.insert( { port, con } );         
        }

        if ( selTrain->isPf() )
        {
            selTrain->setPfGPIO( track38ConfigTrain->Read( "gpio", "13" ) );
            selTrain->setPfChannel( track38ConfigTrain->Read( "channel", "1" ) );
            selTrain->setPfSubChannel( track38ConfigTrain->Read( "subChannel", "R" ) );        
        }

        else if ( selTrain->isUp() )
        {
            selTrain->setHubAdress( track38ConfigTrain->Read( "hubAdress", "" ) );
            selTrain->setUpChannel( track38ConfigTrain->Read( "channel", "1" ) );
            selTrain->setTwoMotors( track38ConfigTrain->Read( "twoMotorsUsed", false ) );
        }
    }
}

void trainControlPanel::OnStopAll( wxCommandEvent& event )
{
    this->StopAll();
}

void trainControlPanel::StopAll()
{
    for (train* & selTrain : this->trains)
    {
        selTrain->Stop();
    }  
}

trainControlPanel::~trainControlPanel()
{
    this->StopAll();
    delete stopAllBtn;
}