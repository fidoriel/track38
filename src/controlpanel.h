#ifndef controleditpanel_h
#define controleditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "traincontrol.h"
#include "traincontrolbox.h"
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
        void updateSwitchStatus();        

        std::unordered_map< wxString, int > cons;
        wxNotebook* parent = nullptr;
        trainControlBox* m_trainControlBox = nullptr;
        // switchControlBox* m_switchControlBox;
        wxBoxSizer* topSizer = nullptr;
        wxBoxSizer* mapSizer = nullptr;
        switchHandler* m_switchHandler = nullptr;
        void initConf();

        // Map
        wxGrid* map = nullptr;
        wxFileConfig* configMap = nullptr;
        wxConfigBase* track38ConfigMap = nullptr;
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