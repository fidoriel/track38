#include "mapeditpanel.h"

wxBEGIN_EVENT_TABLE( mapEditPanel, wxPanel )
    // EVT_BUTTON( ID_REFRESHSERIAL, upEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

mapEditPanel::mapEditPanel( wxNotebook* parent ) : wxPanel( parent )
{
    topSizer = new wxBoxSizer( wxVERTICAL );
    mapSizer = new wxBoxSizer( wxHORIZONTAL );

    map = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    mapX = 50;
    mapY = 30;
    map->CreateGrid( mapY, mapX );
	map->EnableEditing(false);
	map->EnableGridLines(false);
    // map->SetMargins(-1, -1);
	// map->EnableDragGridSize(false);
	// map->EnableDragColMove(true);
	map->EnableDragColSize(false);
	map->HideColLabels();
	map->EnableDragRowSize(false);
	map->HideRowLabels();

    for (int i = 0; i < mapX; i++)
    {
        map->SetColSize(i, 50);
    }

    for (int i = 0; i < mapY; i++)
    {
        map->SetRowSize(i, 50);
    }

    

    picker = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize );
    picker->CreateGrid( 16, 1 );
	picker->EnableEditing(false);
	picker->EnableGridLines(false);
    // picker->SetMargins(-1, -1);
	// picker->EnableDragGridSize(false);
	// picker->EnableDragColMove(true);
	picker->EnableDragColSize(false);
	picker->HideColLabels();
	picker->EnableDragRowSize(false);
	picker->HideRowLabels();

    picker->SetColSize(0, 50);
    for (int i = 0; i < 16; i++)
    {
        picker->SetRowSize(i, 50);
    }

    wxStaticBox* trackSwitchEdit = new wxStaticBox(this, wxID_ANY, "Track Switch Edit");

    mapSizer->Add( map, 15, wxALL, 10 );
    mapSizer->Add( picker, 1, wxALL, 10 );

    topSizer->Add( mapSizer, 4, wxALL, 0 );
    topSizer->Add( trackSwitchEdit, 3, wxALL, 0 );

    this->SetSizer( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();

}