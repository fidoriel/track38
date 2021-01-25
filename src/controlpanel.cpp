#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxFlexGridSizer( 2, 10 , 10);

    m_trainControlBox = new wxStaticBox( this, wxID_ANY, "Train Control", wxDefaultPosition, wxDefaultSize, 0L,  "trainControlBox" );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox" );

    //
    wxStaticBoxSizer* sizer = new wxStaticBoxSizer( m_trainControlBox, wxVERTICAL );
    
    trainControlBox* my_image = new trainControlBox( this, wxID_ANY );
    //

    // topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );
    topSizer->Add( m_switchControlBox, 2, wxGROW | wxALL, 10 );

    m_switchControlBox->loadswitchs( cons );
    m_switchControlBox->createControlBox();

    my_image->loadTrains( cons );
    my_image->createControlBox();
    sizer->Add( my_image->stopAllBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10 );
    sizer->AddSpacer(10);
    sizer->Add( my_image, 0, wxEXPAND | wxALL, 10 );

    topSizer->Add( sizer, 0, wxGROW | wxALL, 10 );

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}

void controlPanel::RefreshPanel()
{
    // this->CloseAll();
    // for (std::pair< wxString, int> element : cons)
    // {
    //     wxMessageBox( element.first );
    //     wxMessageBox( wxString::Format( wxT("%i"), element.second ) );
    // }

    // topSizer->Detach( 0 );
    // topSizer->Detach( 0 );

    // delete m_trainControlBox;
    // delete m_switchControlBox;

    // m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox" );
    // m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox" );
    // topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );
    // topSizer->Add( m_switchControlBox, 2, wxGROW | wxALL, 10 );

    // // m_trainControlBox->deleteTrains();
    // m_trainControlBox->loadTrains( cons );
    // m_trainControlBox->createControlBox();

    // // m_switchControlBox->deleteswitchs();
    // m_switchControlBox->loadswitchs( cons );
    // m_switchControlBox->createControlBox();

    // this->SetSizerAndFit( topSizer );
    // this->Layout();
	// topSizer->Fit( this );
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
        close_port( element.second );
    }
    cons.clear();
}

controlPanel::~controlPanel()
{
}