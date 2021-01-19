#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "traincontrol.h"

class controlPanel : public wxPanel
{ 
    public:
        controlPanel( wxNotebook* parent );
        ~controlPanel();

        void CloseAll();
        void RefreshPanel();

        std::unordered_map< wxString, int > cons;

        trainControlBox* m_trainControlBox;
        wxFlexGridSizer* topSizer;

        wxNotebook* parent;
};

#endif