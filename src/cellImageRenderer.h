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
        cellImageRenderer( wxString filename, int border = 0, int degree = 0 );
        cellImageRenderer();
        
        wxBitmap* renderBmp = nullptr;
        wxString file;
        wxString filepath;
        wxImage image;
        int border;
        int rotation;
        bool isEmptyCell;
        virtual void Draw(wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected) wxOVERRIDE;
};

#endif