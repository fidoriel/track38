#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxFlexGridSizer( 2, 10 , 10);

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox" );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox" );
    topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );
    topSizer->Add( m_switchControlBox, 2, wxGROW | wxALL, 10 );

    m_switchControlBox->loadswitchs( cons );
    m_switchControlBox->createControlBox();

    m_trainControlBox->loadTrains( cons );
    m_trainControlBox->createControlBox();

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}

void controlPanel::RefreshPanel()
{
    this->CloseAll();
    for (std::pair< wxString, int> element : cons)
    {
        wxMessageBox( element.first );
        wxMessageBox( wxString::Format( wxT("%i"), element.second ) );
    }

    topSizer->Detach( 0 );
    topSizer->Detach( 0 );

    delete m_trainControlBox;
    delete m_switchControlBox;

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox" );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox" );
    topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );
    topSizer->Add( m_switchControlBox, 2, wxGROW | wxALL, 10 );

    // m_trainControlBox->deleteTrains();
    m_trainControlBox->loadTrains( cons );
    m_trainControlBox->createControlBox();

    // m_switchControlBox->deleteswitchs();
    m_switchControlBox->loadswitchs( cons );
    m_switchControlBox->createControlBox();

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
}

void controlPanel::OnRefreshPanel( wxCommandEvent& event )
{
    this->RefreshPanel();
}

void controlPanel::CloseAll()
{
    // Iterate over an unordered_map using range based for loop
    for (std::pair< wxString, int> element : cons)
    {
        #if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
        close_port( element.second );
        #endif
    }
    cons.clear();
}

controlPanel::~controlPanel()
{
}