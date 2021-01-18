#include "mapeditpanel.h"

wxBEGIN_EVENT_TABLE( mapEditPanel, wxPanel )
    // EVT_BUTTON( ID_REFRESHSERIAL, upEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

mapEditPanel::mapEditPanel( wxNotebook* parent ) : wxPanel( parent )
{
    topSizer = new wxBoxSizer( wxVERTICAL );
    //mapSizer = new wxBoxSizer( wxHORIZONTAL );
    // switchSizer = new wxBoxSizer( wxHORIZONTAL );

    /*
    map = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    mapX = 10;
    mapY = 10;
    map->CreateGrid( mapY, mapX );
	map->EnableEditing( false );
	map->EnableGridLines( false );
    // map->SetMargins( -1, -1 );
	// map->EnableDragGridSize( false );
	// map->EnableDragColMove( true );
	map->EnableDragColSize( false );
	map->HideColLabels();
	map->EnableDragRowSize( false );
	map->HideRowLabels();

    for ( int i = 0; i < mapX; i++ )
    {
        map->SetColSize(i, 50);
    }

    for ( int i = 0; i < mapY; i++ )
    {
        map->SetRowSize( i, 50 );
    }

    picker = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    picker->CreateGrid( 16, 1 );
	picker->EnableEditing( false );
	picker->EnableGridLines( false );
    // picker->SetMargins(-1, -1);
	// picker->EnableDragGridSize( false );
	// picker->EnableDragColMove( true );
	picker->EnableDragColSize( false );
	picker->HideColLabels();
	picker->EnableDragRowSize( false );
	picker->HideRowLabels();

    picker->SetColSize(0, 50);
    for (int i = 0; i < 16; i++)
    {
        picker->SetRowSize(i, 50);
    }*/

    switchPickerBox = new wxStaticBox( this, wxID_ANY, "Pick Switch to edit" );
    switchPickerBoxSizer = new wxStaticBoxSizer( switchPickerBox, wxVERTICAL );
    m_switchPicker = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL );

    switchPickerBoxSizer->Add( m_switchPicker, 1, wxGROW | wxALL, 5 );
    switchPickerBoxSizer->SetMinSize( 200, 0 );

    /*editBox = new wxStaticBox( this, wxID_ANY, "Pick Switch to edit" );
    editBoxSizer = new wxStaticBoxSizer( editBox, wxVERTICAL );
    m_editPicker = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL );

    editBoxSizer->Add( m_editPicker, 1, wxGROW | wxALL, 5 );*/
    // switchPickerBoxSizer->SetMinSize( 200, 0 );
    
    // mapSizer->Add( map, 15, wxALL | wxEXPAND, 10 );
    // mapSizer->Add( picker, 1, wxALL | wxEXPAND, 10 );

    // topSizer->Add( mapSizer, 4, wxALL, 0 );
    // switchSizer->Add( switchPickerBox, 3, wxGROW | wxALL, 10 );
    // switchSizer->Add( editBox, 3, wxGROW | wxALL, 10 );

    topSizer->Add( switchPickerBoxSizer, 0, wxALL, 0 );

    parent->SetSizerAndFit( topSizer );
    parent->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}