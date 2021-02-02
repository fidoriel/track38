#include "track38Frame.h"

// user wxGetApp()
#include "track38App.h"

//-------------------------
// Macro stuff
//-------------------------
// Event table for track38Frame
BEGIN_EVENT_TABLE( track38Frame, wxFrame )
    EVT_MENU( wxID_ABOUT, track38Frame::OnAbout )
    EVT_MENU( wxID_EXIT,  track38Frame::OnQuit )
    // EVT_MENU( ID_Export,  track38Frame::OnQuit )
    // EVT_MENU( ID_Import,  track38Frame::OnQuit )
    EVT_NOTEBOOK_PAGE_CHANGING( ID_NbChanged, track38Frame::OnNbChangeing )
    EVT_NOTEBOOK_PAGE_CHANGED( ID_NbChanged, track38Frame::OnNbChanged )
END_EVENT_TABLE()

void track38Frame::OnAbout( wxCommandEvent& event )
{
    //Blöng sound
    wxBell();

    //Message window
    wxString msg;
    msg.Printf( "track38 is a LEGO Train Layout Control Software developed by fidoriel. It is able to control LEGO PoweredUp, PowerFunctions and Track Switches via Arduino and esp8266.", wxVERSION_STRING );
    wxString title;
    title.Printf( "About %s", wxGetApp().GetAppName() );
    wxMessageBox( msg, title, wxOK | wxICON_INFORMATION, this );
}

void track38Frame::OnQuit( wxCommandEvent& event )
{
    // Destroy the frame
    Close( true );
}

track38Frame::track38Frame() : wxFrame( NULL, wxID_ANY, wxGetApp().GetAppName()/*, wxPoint( 30, 30 ), wxSize( 200, 200 )*/ )
{
    // Image renderers
    // ::wxInitAllImageHandlers();
    wxImage::AddHandler( new wxPNGHandler );
    
    SetIcon( wxICON( AppIcon ) );

    //----------------
    //Main Application
    //----------------

    //Höchster sizer für AppUI
    topSizer = new wxBoxSizer( wxVERTICAL );

    //Erstellt Tabsystem aus mainPanel
    m_notebook = new wxNotebook( this, ID_NbChanged, wxDefaultPosition, wxDefaultSize, wxFULLSCREEN_ALL );

    //fügt seiten zu Tabsystem hinzu
    m_controlPanel = new controlPanel( m_notebook );
    m_trainEditPanel = new trainEditPanel( m_notebook );
    m_mapEditPanel = new mapEditPanel( m_notebook );
    
    m_notebook->AddPage( m_controlPanel, L"Control" );
    m_notebook->AddPage( m_mapEditPanel, L"Edit Map" );
    m_notebook->AddPage( m_trainEditPanel, L"Edit Trains" );

    topSizer->SetMinSize( minw, minh );
    topSizer->Add( m_notebook, -1, wxGROW | wxEXPAND );
    topSizer->SetSizeHints( this );
    this->SetSizerAndFit( topSizer );
    this->Layout();

    //-----------
    // Status bar
    //-----------
    // CreateStatusBar( 2 );

    //--------------
    // Load Settings
    //--------------

    this->checkDir();

    configMain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + wxGetApp().GetAppName() + ".ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configMain );
    wxConfigBase *track38ConfigBase = wxConfigBase::Get();

    track38ConfigBase->SetPath( "/Application/" );

    // restore frame position and size
    int x = track38ConfigBase->Read( "frameX", 50 ),
        y = track38ConfigBase->Read( "frameY", 50 ),
        w = track38ConfigBase->Read( "frameW", minw ),
        h = track38ConfigBase->Read( "frameH", minh );
    this->Move( x, y );
    this->SetClientSize( w, h );

    track38ConfigBase->SetPath( "/" );

    //----------------
    //Create the menu
    //----------------

    helpMenu = new wxMenu;
    helpMenu->Append( wxID_ABOUT );
    helpMenu->Append( ID_VisitGithub, "GitHub Development Page", "" );
    helpMenu->Append( wxID_PREFERENCES );
    Bind( wxEVT_COMMAND_MENU_SELECTED, &track38Frame::OnSettings, this, wxID_PREFERENCES );

    fileMenu = new wxMenu;
    fileMenu->Append( ID_ImportT, "Import Trains" );
    fileMenu->Enable( ID_ImportT, false );
    fileMenu->Append( ID_ImportMS, "Import Map & Switches" );
    fileMenu->Enable( ID_ImportMS, false );
    fileMenu->AppendSeparator();
    fileMenu->Append( ID_ExportT, "Export Trains" );
    fileMenu->Enable( ID_ExportT, false );
    fileMenu->Append( ID_ExportMS, "Export Map & Switches" );
    fileMenu->Enable( ID_ExportMS, false );
    fileMenu->Append( wxID_EXIT );

    controlMenu = new wxMenu;
    controlMenu->Append( ID_Reconnect, "Reload Connections\tCtrl-R", "Reconnect all Clients" );

    Bind( wxEVT_COMMAND_MENU_SELECTED, &controlPanel::OnRefreshPanel, m_controlPanel, ID_Reconnect );
    Bind( wxEVT_COMMAND_MENU_SELECTED,
        []( wxCommandEvent& ) {
        wxLaunchDefaultBrowser( "https://github.com/fidoriel/track38" );
        }, ID_VisitGithub );

    //--------
    //Menu bar
    //--------

    // Now append the freshly created menu to the menu bar...
    menuBar = new wxMenuBar();
    menuBar->Append( fileMenu, "File" );
    menuBar->Append( controlMenu, "Control" );
    menuBar->Append( helpMenu, "Help" );
    //attach this menu bar to the frame
    SetMenuBar( menuBar );

    // menuBar->Remove( menuBar->FindMenu( "File" ) );

}

void track38Frame::OnNbChangeing( wxBookCtrlEvent& event )
{
    switch ( event.GetOldSelection() )
    {
    case 0:
        {
            wxMessageDialog dialog( this, "By switching to the Edit Panels all trains are going to be stopped", "Stop all Trains", wxYES_NO | wxICON_INFORMATION );
            switch ( dialog.ShowModal() )
            {
                case wxID_YES:
		    // wxMessageBox( "stop" );
                    m_controlPanel->m_trainControlBox->m_trainControlPanel->StopAll();
		    usleep(50000);
                    m_controlPanel->CloseAll();
                    return;
                    break;

                case wxID_NO:
                    event.Veto();
                    return;
                    break;
            }
        }
        break;
    
    case 1:
        m_mapEditPanel->SaveMapToFile();
        m_mapEditPanel->saveSwitch();
        break;
    
    case 2:
        m_trainEditPanel->SaveTrain();
        break;
    
    default:
        break;
    }
}

void track38Frame::OnNbChanged( wxBookCtrlEvent& event )
{
    if ( event.GetSelection() == 0 )
    {
        m_controlPanel->RefreshPanel();
    }
}

void track38Frame::checkDir()
{
    wxString ini_dir = wxGetApp().ini_dir;

    if ( !wxDirExists( ini_dir ) )
    {
        wxDir::Make( ini_dir );
        // wxMessageBox( ini_dir );
    }
}

void track38Frame::OnSettings( wxCommandEvent& event )
{
    wxGetApp().ShowPreferencesEditor( this );
}

track38Frame::~track38Frame()
{
    m_controlPanel->m_trainControlBox->m_trainControlPanel->StopAll();
    usleep( 50000 );

    m_trainEditPanel->SaveTrain();
    m_mapEditPanel->SaveMapToFile();
    m_mapEditPanel->saveSwitch();

    wxConfigBase::Set( configMain );
    wxConfigBase *track38ConfigBase = wxConfigBase::Get();

    // save the frame position
    int x, y, w, h;
    GetClientSize( &w, &h );
    GetPosition( &x, &y );
    track38ConfigBase->Write( "/Application/frameX", x );
    track38ConfigBase->Write( "/Application/frameY", y );
    track38ConfigBase->Write( "/Application/frameW", w );
    track38ConfigBase->Write( "/Application/frameH", h );

    m_controlPanel->CloseAll();
    m_controlPanel->m_switchHandler->~switchHandler();

    track38ConfigBase->Flush();
}
