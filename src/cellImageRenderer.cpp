#include "cellImageRenderer.h"

// cellImageRenderer::cellImageRenderer( std::string filename, int border )
// {
//     cellImageRenderer( wxString( filename ), border );
// }

cellImageRenderer::cellImageRenderer()
{
    this->isEmptyCell = true;
}

cellImageRenderer::cellImageRenderer( wxString filename, int border, int degree )
{
    // -- find --

    ::wxInitAllImageHandlers();

    #ifdef __APPLE__
    wxString path = wxString( "track38.app/Contents/Resources/" );
    #elif defined(__linux__) || defined(__FreeBSD__)
    wxString path = "resources/";
    #elif __WIN32__
    wxString path = "resources/";
    #endif

    filepath = "";

    if ( filename.Find( "|" ) == wxNOT_FOUND )
    {
        this->rotation = degree;
        this->file = filename;

        this->filepath += path;
        this->filepath += filename;
    }

    else
    {
        wxArrayString ary = wxArrayString( wxSplit( filename, '|' ) );

        this->filepath += path;
        this->filepath += ary.Item( 0 );
        this->rotation = wxAtoi( ary.Item( 1 ) );
        this->file = ary.Item( 0 );
        // wxMessageBox(filepath);
    }

    this->border = border;
    this->isEmptyCell = false;

    wxBitmap bitmap( 100, 100 );
    image = bitmap.ConvertToImage();
    image.LoadFile( filepath, wxBITMAP_TYPE_PNG );

    // wxMessageBox(wxString::Format(wxT("%i"), rotation));

    while ( rotation >= 360 )
        rotation -= 360;    
    
    for (size_t i = 0; i < ( rotation / 90 ); i++)
        image = image.Rotate90();

    // wxMessageBox( filepath );
}

void cellImageRenderer::Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected )
{
    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, false);

    if (!this->isEmptyCell)
    {
        int edge;

        if ( grid.GetNumberCols() == 1)
            edge = ( grid.GetColSize( col ) - wxSYS_VSCROLL_X );
        else
            edge = ( grid.GetColSize( col ) ); 

        image.Rescale( edge - border * 2, edge - border * 2, wxIMAGE_QUALITY_BICUBIC );


        wxBitmap* renderBmp = new wxBitmap( image );

        // ---- renderer ----
        dc.SetClippingRegion( rect );
        // adjust y-offset of image to keep in vertical mid
        dc.DrawBitmap( *renderBmp, rect.x + border, rect.y + border + ( rect.height - edge ) / 2,0);
        dc.DestroyClippingRegion();
    }
}