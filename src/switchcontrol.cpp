#include "switchcontrol.h"

switchControlBox::switchControlBox( wxPanel* parent, int id, wxString title, wxString boxName ) : wxStaticBox( parent, id, title, wxDefaultPosition, wxDefaultSize, 0L, boxName )
{
    this->parent = parent;

    //Sizer
    topSizer = new wxBoxSizer( wxHORIZONTAL );
}

void switchControlBox::deleteswitchs()
{
    while ( !topSizer->IsEmpty() )
    {
        topSizer->Detach( 0 );
    }

    while ( !switches.empty() )
    {
        switches.front()->CloseCon();
        while ( !switches.front()->sizer->IsEmpty() )
        {
            switches.front()->sizer->Detach( 0 );
        }
        
        delete switches.front()->tswitchName;
        delete switches.front()->straightBtn;
        delete switches.front()->turnBtn;
        delete switches.front()->sizer;
        delete switches.front();
        switches.pop_front();
    }
}

void switchControlBox::createControlBox()
{
    for (tswitch* & selswitch : this->switches)
    {
        selswitch->createControls(this);

        selswitch->sizer->Add( selswitch->tswitchName, 1, wxALL, 10 );
        selswitch->sizer->Add( selswitch->straightBtn, 1, wxALL , 10 );
        selswitch->sizer->Add( selswitch->turnBtn, 1, wxALL , 10 );

        topSizer->Add(selswitch->sizer);

        selswitch->straightBtn->Bind( wxEVT_BUTTON, &tswitch::OnStraight, selswitch );
        selswitch->turnBtn->Bind( wxEVT_BUTTON, &tswitch::OnTurn, selswitch );
    }

    this->SetSizer( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();
}

void switchControlBox::loadswitchs( std::unordered_map< wxString, int > &cons )
{
    //init Config
    wxConfigBase* track38Configswitch = wxConfigBase::Get();
    track38Configswitch->SetPath( "/Switch/" );
    switches.clear();

    // Read switch Names
    long idx;
    wxString out;
    bool exists = track38Configswitch->GetFirstGroup( out, idx );
    if (  exists == true  )
        switches.push_back( new tswitch(out) );
    
    while ( exists )
    {
        exists = track38Configswitch->GetNextGroup( out, idx );
        if (  exists == true  )
            switches.push_back( new tswitch(out) );
    }

    //Fill tain information
    for (tswitch* & selswitch : switches)
    {
        // Access the object through iterator
        track38Configswitch->SetPath( "/Switch/" );
        track38Configswitch->SetPath( selswitch->name );

        wxString port = track38Configswitch->Read( "port", "" );
        selswitch->setPort( port );
        selswitch->setGPIO( track38Configswitch->Read( "gpio", "" ) );
        selswitch->setDir( track38Configswitch->Read( "dir", "" ) );
        selswitch->setManufacturer( track38Configswitch->Read( "manufacturer", "" ) );

        #if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    
        if ( cons.count( port ) )
        {
            wxMessageBox( "port used" );
            selswitch->con = cons[ port ];
        }
        
        else
        {
            int con = connect_port( selswitch->portPoint );

            if ( con < 0 )
            {
                usleep( 2000000 );
                con = connect_port( selswitch->portPoint );
                if ( con < 0 )
                {
                    wxString msg;
                    msg.Printf( "The port used by %s is not avaliable. Please select an other port or plug in the device.", selswitch->getName().c_str() );
                    wxMessageBox( msg, "switch Port not avaliable");
                }
            }

            selswitch->con = con;
        }
        #endif
    }
}

switchControlBox::~switchControlBox()
{
}