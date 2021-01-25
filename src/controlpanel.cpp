#include "controlpanel.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxBoxSizer( wxHORIZONTAL);
    mapSizer = new wxBoxSizer( wxVERTICAL );

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox", cons );

    this->CreateMap();

    mapSizer->Add( map, 1, wxGROW | wxALL, 10 );
    mapSizer->Add( m_switchControlBox->sizer, 0, wxGROW | wxALL, 10 );
    topSizer->Add( m_trainControlBox->sizer, 0, wxGROW | wxALL, 10 );  
    topSizer->Add( mapSizer, 1, wxGROW | wxALL, 10 ); 

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

    while ( mapSizer->GetItemCount() )
    {
        mapSizer->Detach( 0 );
    }

    while ( topSizer->GetItemCount() )
    {
        topSizer->Detach( 0 );
    }

    delete map;
    delete m_trainControlBox;
    delete m_switchControlBox;

    this->CreateMap();
    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox", cons );
    
    mapSizer->Add( map, 1, wxGROW | wxALL, 10 );
    mapSizer->Add( m_switchControlBox->sizer, 0, wxGROW | wxALL, 10 );
    topSizer->Add( m_trainControlBox->sizer, 0, wxGROW | wxALL, 10 );  
    topSizer->Add( mapSizer, 1, wxGROW | wxALL, 10 ); 

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();
}

void controlPanel::CreateMap()
{
    map = new wxGrid( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 40, 40) );

    map->CreateGrid( 25, 50);
    map->HideColLabels();
    map->HideRowLabels();
    map->EnableEditing( false );
    map->EnableGridLines( false );
    map->EnableDragGridSize( false );
    map->DisableCellEditControl();
    map->SetMargins( 0 , 0 );
    map->SetCellHighlightColour( *wxWHITE );
    map->SetCellHighlightROPenWidth( 0 );

    for ( size_t i = 0; i < map->GetNumberRows(); i++ )
    {
        map->SetRowSize( i, 50 );
    }

    for ( size_t i = 0; i < map->GetNumberCols(); i++ )
    {
        map->SetColSize( i, 50 );
    }

    for ( size_t col = 0; col < map->GetNumberCols(); col++ )
    {
        for ( size_t row = 0; row < map->GetNumberRows(); row++ )
        {
            map->SetReadOnly( row, col );
            map->SetCellBackgroundColour( row, col, *wxWHITE );
            map->SetCellTextColour( row, col, *wxBLACK );
            map->SetCellRenderer( row, col, new cellImageRenderer() );
        }
    }
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