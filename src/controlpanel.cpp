#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxFlexGridSizer( 2, 10 , 10);

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox" );
    topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );

    m_trainControlBox->loadTrains( cons );
    m_trainControlBox->createControlBox();

    parent->SetSizerAndFit( topSizer );
    parent->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}

void controlPanel::RefreshPanel()
{
    m_trainControlBox->deleteTrains();
    m_trainControlBox->loadTrains( cons );
    m_trainControlBox->createControlBox();
}

void controlPanel::CloseAll()
{
    // Iterate over an unordered_map using range based for loop
    for (std::pair< wxString, int> element : cons)
    {
        close_port( element.second );
    }
}

controlPanel::~controlPanel()
{
}