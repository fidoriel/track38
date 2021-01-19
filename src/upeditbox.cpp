#include "upeditbox.h"

wxBEGIN_EVENT_TABLE( upEditBox, wxPanel )
    EVT_BUTTON( ID_REFRESHSERIAL, upEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

upEditBox::upEditBox( wxPanel* parent, int id, wxString title ) : wxStaticBox( parent, id, title )
{
    topSizerUpEdit = new wxFlexGridSizer( 2, 0, 0 );

    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "upName" );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    portTxt = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "upPort" );

    m_RefreshBtn = new wxButton( this, ID_REFRESHSERIAL, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 10 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 10 );

    hubAdressTxt = new wxStaticText( this, wxID_ANY, "Hub Adress:" );
    hubAdress = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "upHubAdress" );

    channelTxt = new wxStaticText( this, wxID_ANY, "PoweredUP Hub Channel:" );
    channel.Add( "A" );
    channel.Add( "B" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "upChannel" );

    hasTwoMotorsTxt = new wxStaticText( this, wxID_ANY, "Two Motors used:" );
    hasTwoMotors = new wxCheckBox( this, wxID_ANY, wxT( "" ), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, "upAreTwoMotorsUsed" );

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "7", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 7, "upSpeed" );

    topSizerUpEdit->AddSpacer( 10 );
    topSizerUpEdit->AddSpacer( 10 );
    
    topSizerUpEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerUpEdit->Add( portTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    topSizerUpEdit->Add( hubAdressTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( hubAdress, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerUpEdit->Add( channelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( channelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerUpEdit->Add( hasTwoMotorsTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( hasTwoMotors, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerUpEdit->Add( maxSpeedTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    topSizerUpEdit->Add( maxSpeedPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    topSizerUpEdit->AddSpacer( 10 );
    topSizerUpEdit->AddSpacer( 10 );
    
    // Top Sizer Settings
    parent->SetSizer( topSizerUpEdit );
    parent->Layout();
	topSizerUpEdit->Fit( this );
    topSizerUpEdit->SetSizeHints( this );
}

void upEditBox::OnRefreshSerial( wxCommandEvent& event )
{
    refreshSizer->Detach( 0 );
    delete portPicker;
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "upPort" );
    refreshSizer->Insert( 0, portPicker, 0, wxALL, 10 );
    refreshSizer->Layout();
}

void upEditBox::refreshSerial()
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
            if ( strstr( directory->d_name, "cu.usb" ) != NULL )
            #endif
            #if defined( __linux__ ) || defined( __FreeBSD__ )
            if ( strstr( directory->d_name, "ttyUSB" ) != NULL )
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