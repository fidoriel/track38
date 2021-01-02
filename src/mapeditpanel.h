#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel(wxNotebook* parent);
};

#endif