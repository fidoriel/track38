#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxFlexGridSizer( 2, 10 , 10);

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox", cons );

    topSizer->Add( m_switchControlBox->sizer, 2, wxGROW | wxALL, 10 );
    topSizer->Add( m_trainControlBox->sizer, 2, wxGROW | wxALL, 10 );    

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

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox", cons );
    
    topSizer->Add( m_trainControlBox->sizer, 2, wxGROW | wxALL, 10 );   
    topSizer->Add( m_switchControlBox->sizer, 2, wxGROW | wxALL, 10 ); 

    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
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