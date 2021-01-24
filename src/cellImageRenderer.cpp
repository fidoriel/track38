#include "cellImageRenderer.h"

cellImageRenderer::cellImageRenderer( std::string filename, int border )
{
    // -- find --

    ::wxInitAllImageHandlers();

    #ifdef __APPLE__
    wxString path = wxString( "track38.app/Contents/Resources/tracks/" );
    #elif defined(__linux__) || defined(__FreeBSD__)
    wxString path = "resources/";
    #elif __WIN32__
    wxString path = "resources/";
    #endif

    filepath += path;
    filepath += wxString( filename );

    this->border = border;

    // wxMessageBox( filepath );
}

void cellImageRenderer::Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected )
{
    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);
    wxBitmap bitmap( 100, 100 );
    wxImage image = bitmap.ConvertToImage();

    // --- PNG ---

    int edge;

    if ( grid.GetNumberCols() == 1)
        edge = ( grid.GetColSize( col ) - wxSYS_VSCROLL_X );
    else
        edge = ( grid.GetColSize( col ) ); 

    image.LoadFile( filepath, wxBITMAP_TYPE_PNG );
    image.Rescale( edge - border * 2, edge - border * 2, wxIMAGE_QUALITY_BICUBIC );


    wxBitmap* renderBmp = new wxBitmap( image );

    // ---- renderer ----
    dc.SetClippingRegion( rect );
    // adjust y-offset of image to keep in vertical mid
    dc.DrawBitmap( *renderBmp, rect.x + border, rect.y + border + ( rect.height - edge ) / 2,0);
    dc.DestroyClippingRegion();
}