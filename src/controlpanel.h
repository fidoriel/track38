#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "traincontrol.h"
#include "switchcontrol.h"
#include "traincontrolbox.h"
#include "switchcontrolbox.h"
#include "cellImageRenderer.h"
#include "switch.h"
#include "switchhandler.h"

class controlPanel : public wxPanel
{ 
    public:
        controlPanel( wxNotebook* parent );
        ~controlPanel();

        void CloseAll();
        void RefreshPanel();
        void OnRefreshPanel( wxCommandEvent& event );
        

        std::unordered_map< wxString, int > cons;
        wxNotebook* parent;
        trainControlBox* m_trainControlBox;
        switchControlBox* m_switchControlBox;
        wxBoxSizer* topSizer;
        wxBoxSizer* mapSizer;
        switchHandler* m_switchHandler;
        void initConf();

        // Map
        wxGrid* map;
        wxFileConfig* configMap;
        wxConfigBase* track38ConfigMap;
        void LoadMapFromFile();
        void OnLClickMap( wxGridEvent& event );
        void RefreshMap();

        enum
        {
            ID_MapLClick,
            ID_SwitchChange
        };
};

#endif