#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel( wxNotebook* parent );


        wxBoxSizer* topSizer;
        wxBoxSizer* mapSizer;
        wxGrid* map;
        wxGrid* picker;

        int mapX;
        int mapY;

    private:
    DECLARE_EVENT_TABLE()
};

#endif