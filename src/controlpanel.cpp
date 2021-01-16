#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxFlexGridSizer( 2, 10 , 10);

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox" );
    topSizer->Add( m_trainControlBox, 2, wxGROW | wxALL, 10 );

    parent->SetSizerAndFit( topSizer );
    parent->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}

controlPanel::~controlPanel()
{
}