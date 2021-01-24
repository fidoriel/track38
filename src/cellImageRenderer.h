#ifndef cellImageRenderer_h
#define cellImageRenderer_h

#include "wx/wx.h"
#include "wx/grid.h"
#include "wx/image.h"
#include "wx/string.h"

#include <string>

class cellImageRenderer : public wxGridCellStringRenderer
{
public:
    // cellImageRenderer( std::string filename, int border = 0 );
    cellImageRenderer( wxString filename, int border = 0 );
    wxBitmap* renderBmp;
    wxImage* image;
    wxString file;
    wxString filepath;
    int border;
    int degree;
    virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) wxOVERRIDE;
};

#endif