#include "wx/wx.h"
#include <string>
#include "wx/fileconf.h"
#include "wx/config.h"

#include "../icons/AppIcon.xpm"

#include "traineditpanel.h"
#include "mapeditpanel.h"
#include "controlpanel.h"

//-------------------------
// Class/Function declaration
//-------------------------

class track38App : public wxApp
{
public:
  virtual bool OnInit();
};


class track38Frame : public wxFrame
{
public:
    track38Frame();
    ~track38Frame();

    wxString name = "track38";

    // Event handlers
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    int minw = 900;
    int minh = 650;

private:
    // This class handles events
    DECLARE_EVENT_TABLE()
};

//-------------------------
// Macro stuff
//-------------------------
// Event table for track38Frame
BEGIN_EVENT_TABLE( track38Frame, wxFrame )
    EVT_MENU( wxID_ABOUT, track38Frame::OnAbout )
    EVT_MENU( wxID_EXIT,  track38Frame::OnQuit )
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

void track38Frame::OnAbout( wxCommandEvent& event )
{
    //Blöng sound
    wxBell();

    //Message window
    wxString msg;
    msg.Printf( "Hello and welcome to %s", wxVERSION_STRING );
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

    SetIcon( wxICON( AppIcon ) );

    //----------------
    //Create the menu
    //----------------

    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;

    helpMenu->Append( wxID_ABOUT, "&About...\tF1", "Show about dialog" );
    fileMenu->Append( wxID_EXIT, "&Exit\tAlt-X", "Quit this program" );
    fileMenu->Append( wxID_OPEN, "&Open\tCtrl-O", "Open a .t42 file" );

    //--------
    //Menu bar
    //--------

    // Now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append( fileMenu, "&File" );
    menuBar->Append( helpMenu, "&Help" );
    //attach this menu bar to the frame
    SetMenuBar( menuBar );


    //----------------
    //Main Application
    //----------------

    //Höchster sizer für AppUI
    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );

    //Erstellt Tabsystem aus mainPanel
    wxNotebook* m_notebook;
    m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFULLSCREEN_ALL );

    //fügt seiten zu Tabsystem hinzu
    controlPanel* m_controlPanel = new controlPanel( m_notebook );
    trainEditPanel* m_trainEditPanel = new trainEditPanel( m_notebook );
    mapEditPanel* m_mapEditPanel = new mapEditPanel( m_notebook );
    
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

    wxConfigBase *track38ConfigBase = wxConfigBase::Get();

    track38ConfigBase->SetPath( "/Application" );

    // restore frame position and size
    int x = track38ConfigBase->Read( "frameX", 50 ),
        y = track38ConfigBase->Read( "frameY", 50 ),
        w = track38ConfigBase->Read( "frameW", minw ),
        h = track38ConfigBase->Read( "frameH", minh );
    Move( x, y );
    SetClientSize( w, h );

    track38ConfigBase->SetPath( "/" );
}

track38Frame::~track38Frame()
{
    wxConfigBase *track38ConfigBase = wxConfigBase::Get();
    if ( track38ConfigBase == NULL )
    {
        return;
    }

    // save the frame position
    int x, y, w, h;
    GetClientSize( &w, &h );
    GetPosition( &x, &y );
    track38ConfigBase->Write( "/Application/frameX", x );
    track38ConfigBase->Write( "/Application/frameY", y );
    track38ConfigBase->Write( "/Application/frameW", w );
    track38ConfigBase->Write( "/Application/frameH", h );
}