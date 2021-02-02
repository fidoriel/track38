#include "switchhandler.h"
#include "track38App.h"

switchHandler::switchHandler()
{
}

void switchHandler::loadswitchs( std::unordered_map< wxString, int > &cons )
{
    // init Config
    configMap = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "map.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configMap );
    track38ConfigMap = wxConfigBase::Get();

    //init Config
    track38ConfigMap->SetPath( "/Switch/" );
    switches.clear();

    // Read switch Names
    long idx;
    wxString out;
    bool exists = track38ConfigMap->GetFirstGroup( out, idx );
    if (  exists == true  )
        switches.push_back( new tswitch(out) );
    
    while ( exists )
    {
        exists = track38ConfigMap->GetNextGroup( out, idx );
        if (  exists == true  )
            switches.push_back( new tswitch(out) );
    }

    //Fill tain information
    for (tswitch* & selswitch : switches)
    {
        // Access the object through iterator
        track38ConfigMap->SetPath( "/Switch/" );
        track38ConfigMap->SetPath( selswitch->name );

        wxString port = track38ConfigMap->Read( "port", "" );
        selswitch->setPort( port );
        selswitch->setGPIO( track38ConfigMap->Read( "gpio", "" ) );
        selswitch->setDir( track38ConfigMap->Read( "dir", "" ) );
        selswitch->setManufacturer( track38ConfigMap->Read( "manufacturer", "" ) );
        selswitch->SetMapRowCol( wxAtoi( track38ConfigMap->Read( "row", "" ) ), wxAtoi( track38ConfigMap->Read( "col", "" ) ) );
        selswitch->setCurrentPos( track38ConfigMap->Read( "currentPos", "n" ) );

    
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

switchHandler::~switchHandler()
{
    // init Config
    configMap = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "map.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configMap );
    track38ConfigMap = wxConfigBase::Get();

    for (tswitch* & selswitch : switches)
    {
        // Access the object through iterator
        track38ConfigMap->SetPath( "/Switch/" );
        if ( track38ConfigMap->HasGroup( selswitch->name ) )
        {
            track38ConfigMap->SetPath( selswitch->name );
            track38ConfigMap->Write( "currentPos", wxString( selswitch->currentPos ) );
        }
    }
    track38ConfigMap->Flush();
}