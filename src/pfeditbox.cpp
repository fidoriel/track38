#include "pfeditbox.h"

wxBEGIN_EVENT_TABLE( pfEditBox, wxPanel )
    EVT_BUTTON( ID_REFRESHSERIAL, pfEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

pfEditBox::pfEditBox( wxPanel* parent, int id, wxString title ) : wxStaticBox( parent, id, title )
{   
    topSizerPfEdit = new wxFlexGridSizer( 2, 0, 0 );


    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "pfName" );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    portTxt = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "pfPort" );

    m_RefreshBtn = new wxButton( this, ID_REFRESHSERIAL, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 10 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 10 );

    gpioTxt = new wxStaticText( this, wxID_ANY, "GPIO:" );
    gpioPicker = new wxSpinCtrl( this, wxID_ANY, "13", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 13, "pfGpio" );

    channelTxt = new wxStaticText( this, wxID_ANY, "PowerFunctions Channel:" );
    channel.Add( "1" );
    channel.Add( "2" );
    channel.Add( "3" );
    channel.Add( "4" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "pfChannel" );

    subChannelTxt = new wxStaticText( this, wxID_ANY, "PowerFunctions SubCannel:" );
    subChannel.Add( "Red" );
    subChannel.Add( "Blue" );
    subChannelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, subChannel, 0L, wxDefaultValidator, "pfSubChannel" );

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7, 5, "pfSpeed" );

    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerPfEdit->Add( portTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    topSizerPfEdit->Add( gpioTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( gpioPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerPfEdit->Add( channelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( channelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerPfEdit->Add( subChannelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( subChannelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerPfEdit->Add( maxSpeedTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerPfEdit->Add( maxSpeedPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->AddSpacer( 10 );
    
    // Top Sizer Settings
    parent->SetSizer( topSizerPfEdit );
    parent->Layout();
	topSizerPfEdit->Fit( this );
    topSizerPfEdit->SetSizeHints( this );
}

void pfEditBox::OnRefreshSerial( wxCommandEvent& event )
{
    refreshSizer->Detach( 0 );
    delete portPicker;
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "pfPort" );
    refreshSizer->Insert( 0, portPicker, 0, wxALL, 10 );
    refreshSizer->Layout();
}

void pfEditBox::refreshSerial()
{
    #if defined( __linux__ ) || defined( __FreeBSD__ ) || defined( __APPLE__ )
        this->serialArray.Empty();
        int comportsElements;
        char directory_name[  ] = "/dev/";
        DIR *ptr;
        struct dirent *directory;
        ptr = opendir( directory_name );
        while( ( directory = readdir( ptr ) ) != NULL )
        {
            #if defined( __APPLE__ )
            if ( /*( strstr( directory->d_name, "tty" ) != NULL ) ||*/ ( strstr( directory->d_name, "cu" ) != NULL ) )
            #endif
            #if defined( __linux__ ) || defined( __FreeBSD__ )
            if ( ( strstr( directory->d_name, "tty" ) != NULL ) || ( strstr( directory->d_name, "cu" ) != NULL ) )
            #endif
            {
                char tmp[ 100 ] = "/dev/";
                for ( int i = 0; i < 95; i++ )
                {
                    tmp[ 5+i ] = directory->d_name[ i ];
                }
                this->serialArray.Add( tmp );
            }
        }
    #endif

    #if defined( WIN32 )
        this->serialArray.Empty();
        wchar_t lpTargetPath[ 5000 ]; // buffer to store the path of the COMPORTS

        for ( int i = 0; i < 255; i++ ) // checking ports from COM0 to COM255
        {
            std::wstring str = L"COM" + std::to_wstring( i ); // converting to COM0, COM1, COM2
            DWORD res = QueryDosDevice( str.c_str(), lpTargetPath, 5000 );

            // Test the return value and error if any
            if ( res != 0 ) //QueryDosDevice returns zero if it didn't find an object
            {
                //std::cout << str << ": " << lpTargetPath << std::endl;
                this->serialArray.Add( str );
            }
            if ( ::GetLastError() == ERROR_INSUFFICIENT_BUFFER )
            {
            }
        }
    #endif
}