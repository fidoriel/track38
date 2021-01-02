#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include <wx/statbox.h>

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel(wxNotebook* parent);
};

mapEditPanel::mapEditPanel(wxNotebook* parent) : wxPanel(parent)
{
    wxFlexGridSizer* editSizer = new wxFlexGridSizer(2, 2, 10, 10);

    //-------------------------
    // Creates a new grid child of panel
    //-------------------------
    
    wxGrid* map = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    // Grid
	map->CreateGrid(9, 9);
	map->EnableEditing(true);
	map->EnableGridLines(true);
    map->SetMargins(-2, -2);
	map->EnableDragGridSize(false);

    map->SetGridLineColour(*wxBLACK);

    // Col
	map->EnableDragColMove(false);
	map->EnableDragColSize(false);
	map->HideColLabels();

	// Row
	map->EnableDragRowSize(false);
	map->HideRowLabels();
       
    // Set sizes
    int cellSize = 50;
    for (int i = 0; i < 9; i++)
    {
        map->SetColSize(i, cellSize);
        map->SetRowSize(i, cellSize);
    }

    for (int a = 0; a < 9; a++)
    {
        for (int  b = 0; b < 9; b++)
        {
            map->SetCellFont(a, b, wxFontInfo(40));
        }
    }

	map->SetDefaultCellAlignment(wxALIGN_CENTER, wxALIGN_CENTER);


    // add the grid and buttons to the sizer

    editSizer->Add(map, 1, wxALL | wxEXPAND, 10 );

    parent->SetSizer(editSizer);
    parent->Layout();
	editSizer->Fit(this);
    editSizer->SetSizeHints(this);
}
