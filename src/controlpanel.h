#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "traincontrol.h"
#include "switchcontrol.h"

class controlPanel : public wxPanel
{ 
    public:
        controlPanel( wxNotebook* parent );
        ~controlPanel();

        void CloseAll();
        void RefreshPanel();
        void OnRefreshPanel( wxCommandEvent& event );

        std::unordered_map< wxString, int > cons;

        wxStaticBox* m_trainControlBox;
        switchControlBox* m_switchControlBox;
        wxFlexGridSizer* topSizer;

        wxNotebook* parent;
};

#endif