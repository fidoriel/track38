#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "traincontrol.h"

class controlPanel : public wxPanel
{ 
    public:
        controlPanel(wxNotebook* parent);

        trainControlBox* m_trainControlBox;
        
};

#endif