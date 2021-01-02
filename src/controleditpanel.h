#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"

class controlEditPanel : public wxPanel
{ 
    public:
        controlEditPanel(wxNotebook* parent);
};

#endif