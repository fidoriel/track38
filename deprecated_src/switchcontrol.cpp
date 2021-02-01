#include "switchcontrol.h"
#include "track38App.h"

switchControlPanel::switchControlPanel( wxPanel* parent, int id ) : wxScrolledWindow( parent, id )
{
    this->parent = parent;

    //Sizer
    topSizer = new wxBoxSizer( wxHORIZONTAL );
}

void switchControlPanel::createControlBox()
{
    for (tswitch* & selswitch : this->switches)
    {
        selswitch->createControls( this );
        
        selswitch->sizer->Add( selswitch->tswitchName, 0, wxALIGN_CENTER | wxALL, 5 );
        selswitch->sizer->Add( selswitch->straightBtn, 0, wxALL , 5 );
        selswitch->sizer->Add( selswitch->turnBtn, 0, wxALL , 5 );

        topSizer->Add( selswitch->sizer );

        selswitch->straightBtn->Bind( wxEVT_BUTTON, &tswitch::OnStraight, selswitch );
        selswitch->turnBtn->Bind( wxEVT_BUTTON, &tswitch::OnTurn, selswitch );
    }

    this->Layout();
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();

    this->SetSizer(topSizer);
    this->FitInside();
    this->SetScrollRate(20, 20);
}

void switchControlPanel::loadswitchs( std::unordered_map< wxString, int > &cons )
{
    // init Config
    configSwitch = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "switches.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configSwitch );
    track38ConfigSwitch = wxConfigBase::Get();

    //init Config
    track38ConfigSwitch->SetPath( "/Switch/" );
    switches.clear();

    // Read switch Names
    long idx;
    wxString out;
    bool exists = track38ConfigSwitch->GetFirstGroup( out, idx );
    if (  exists == true  )
        switches.push_back( new tswitch(out) );
    
    while ( exists )
    {
        exists = track38ConfigSwitch->GetNextGroup( out, idx );
        if (  exists == true  )
            switches.push_back( new tswitch(out) );
    }

    //Fill tain information
    for (tswitch* & selswitch : switches)
    {
        // Access the object through iterator
        track38ConfigSwitch->SetPath( "/Switch/" );
        track38ConfigSwitch->SetPath( selswitch->name );

        wxString port = track38ConfigSwitch->Read( "port", "" );
        selswitch->setPort( port );
        selswitch->setGPIO( track38ConfigSwitch->Read( "gpio", "" ) );
        selswitch->setDir( track38ConfigSwitch->Read( "dir", "" ) );
        selswitch->setManufacturer( track38ConfigSwitch->Read( "manufacturer", "" ) );
        selswitch->SetMapRowCol( wxAtoi( track38ConfigSwitch->Read( "row", "" ) ), wxAtoi( track38ConfigSwitch->Read( "col", "" ) ) );
        selswitch->setCurrentPos( track38ConfigSwitch->Read( "currentPos", "n" ) );

    
        if ( cons.count( port ) )
        {
            // wxMessageBox( "port used" );
            selswitch->con = cons[ port ];
        }
        
        else
        {
            int con = connect_port( selswitch->portPoint );

            if ( con < 0 )
            {
                // usleep( 2000000 );
                con = connect_port( selswitch->portPoint );
                if ( con < 0 )
                {
                    wxString msg;
                    msg.Printf( "The port used by %s is not avaliable. Please select an other port or plug in the device.", selswitch->port );
                    wxMessageBox( msg, "Port not avaliable");
                }
            }

            selswitch->con = con;
            // wxMessageBox( wxString::Format( wxT( "%i" ), con ) );
            cons.insert( { port, con } );
        }
    }
}

switchControlPanel::~switchControlPanel()
{
    // init Config
    configSwitch = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "switches.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configSwitch );
    track38ConfigSwitch = wxConfigBase::Get();

    for (tswitch* & selswitch : switches)
    {
        // Access the object through iterator
        track38ConfigSwitch->SetPath( "/Switch/" );
        if ( track38ConfigSwitch->HasGroup( selswitch->name ) )
        {
            track38ConfigSwitch->SetPath( selswitch->name );
            track38ConfigSwitch->Write( "currentPos", wxString( selswitch->currentPos ) );
        }
    }
    track38ConfigSwitch->Flush();
}