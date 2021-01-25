#include "wx/wx.h"
#include <string>
#include "wx/fileconf.h"
#include "wx/config.h"
#include "wx/image.h"
#include "wx/preferences.h"
#include "wx/scopedptr.h"
#include "wx/artprov.h"

#include "../icons/AppIcon.xpm"

#include "traineditpanel.h"
#include "mapeditpanel.h"
#include "controlpanel.h"
#include "preferences.h"

//-------------------------
// Class/Function declaration
//-------------------------

class track38App : public wxApp
{
public:
    virtual bool OnInit();
    void ShowPreferencesEditor( wxWindow* parent );

private:
    wxScopedPtr<wxPreferencesEditor> m_prefEditor;
};


class track38Frame : public wxFrame
{
public:
    track38Frame();
    ~track38Frame();

    wxString name = "track38";

    // Event handlers
    void OnNbChangeing( wxBookCtrlEvent& event );
    void OnNbChanged( wxBookCtrlEvent& event );
    void OnSettings( wxCommandEvent& event );
    void Settings();

    wxMenu* fileMenu;
    wxMenu* helpMenu;
    wxMenu* controlMenu;

    wxNotebook* m_notebook;

    wxMenuBar* menuBar;
    wxBoxSizer* topSizer;
    controlPanel* m_controlPanel;
    trainEditPanel* m_trainEditPanel;
    mapEditPanel* m_mapEditPanel;

    int minw = 900;
    int minh = 650;

    enum
    {
        ID_NbChanged,
        ID_VisitGithub,
        ID_Reconnect
    };

private:
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()
};

//-------------------------
// Macro stuff
//-------------------------
// Event table for track38Frame
BEGIN_EVENT_TABLE( track38Frame, wxFrame )
    EVT_MENU( wxID_ABOUT, track38Frame::OnAbout )
    EVT_MENU( wxID_EXIT,  track38Frame::OnQuit )
    EVT_NOTEBOOK_PAGE_CHANGING( ID_NbChanged, track38Frame::OnNbChangeing )
    EVT_NOTEBOOK_PAGE_CHANGED( ID_NbChanged, track38Frame::OnNbChanged )
END_EVENT_TABLE()

// Implements track38& GetApp()
wxDECLARE_APP( track38App );
// Give wxWidgets the means to create a track38 object
wxIMPLEMENT_APP( track38App );

//-------------------------
// function definition
//-------------------------

bool track38App::OnInit()
{
    //SetVendorName( "fidoriel" );
    //SetAppName( "track38" );
    track38Frame* m_frame = new track38Frame();
    m_frame->Show();
    return true;
}

void track38App::ShowPreferencesEditor( wxWindow* parent )
{
    if ( !m_prefEditor )
    {
        m_prefEditor.reset( new wxPreferencesEditor );
        m_prefEditor->AddPage( new track38PreferencePageGeneral );
    }

    m_prefEditor->Show( parent );
}

void track38Frame::OnAbout( wxCommandEvent& event )
{
    //Blöng sound
    wxBell();

    //Message window
    wxString msg;
    msg.Printf( "track38 is a LEGO Train Layout Control Software developed by fidoriel. It is able to control LEGO PoweredUp, PowerFunctions and Track Switches via Arduino and esp8266.", wxVERSION_STRING );
    wxString title;
    title.Printf( "About %s", name );
    wxMessageBox( msg, title, wxOK | wxICON_INFORMATION, this );
}

void track38Frame::OnQuit( wxCommandEvent& event )
{
    // Destroy the frame
    Close( true );
}

track38Frame::track38Frame() : wxFrame( NULL, wxID_ANY, "track38"/*, wxPoint( 30, 30 ), wxSize( 200, 200 )*/ )
{

    this->Settings();

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
    //Status bar
    //-----------
    CreateStatusBar( 2 );

    // 
    // Configuration File
    //
    wxConfigBase *track38ConfigBase = wxConfigBase::Get();

    track38ConfigBase->SetPath( "/Application/" );

    // restore frame position and size
    int x = track38ConfigBase->Read( "frameX", 50 ),
        y = track38ConfigBase->Read( "frameY", 50 ),
        w = track38ConfigBase->Read( "frameW", minw ),
        h = track38ConfigBase->Read( "frameH", minh );
    Move( x, y );
    SetClientSize( w, h );

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
    fileMenu->Append( wxID_EXIT );
    fileMenu->Append( wxID_OPEN, "Open\tCtrl-O", "Open a .t38 file" );

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
    if ( event.GetOldSelection() == 0 )
    {
        wxMessageDialog dialog( this, "By switching to the Edit Panel all trains are going to be stopped", "Stop all Trains", wxYES_NO | wxICON_INFORMATION );
        switch ( dialog.ShowModal() )
        {
            case wxID_YES:
                m_controlPanel->m_trainControlBox->m_trainControlPanel->StopAll();
                m_controlPanel->CloseAll();
                return;
                break;

            case wxID_NO:
                event.Veto();
                return;
                break;
        }
    }
}

void track38Frame::OnNbChanged( wxBookCtrlEvent& event )
{
    if ( event.GetSelection() == 0 )
    {
        m_controlPanel->RefreshPanel();
    }
}

void track38Frame::Settings()
{
    wxConfigBase *track38ConfigBase = wxConfigBase::Get();
    if ( track38ConfigBase == NULL )
        return;

    track38ConfigBase->Write( "/ControlSettings/pfRepeatCmd", 5 );   
}

void track38Frame::OnSettings( wxCommandEvent& event )
{
    wxGetApp().ShowPreferencesEditor( this );
}

track38Frame::~track38Frame()
{
    m_controlPanel->m_trainControlBox->m_trainControlPanel->StopAll();
    m_controlPanel->CloseAll();

    wxConfigBase *track38ConfigBase = wxConfigBase::Get();
    if ( track38ConfigBase == NULL )
        return;

    // save the frame position
    int x, y, w, h;
    GetClientSize( &w, &h );
    GetPosition( &x, &y );
    track38ConfigBase->Write( "/Application/frameX", x );
    track38ConfigBase->Write( "/Application/frameY", y );
    track38ConfigBase->Write( "/Application/frameW", w );
    track38ConfigBase->Write( "/Application/frameH", h );
}