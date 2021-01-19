#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "wx/spinctrl.h"
#include "wx/fileconf.h"

#if defined( __linux__ ) || defined( __FreeBSD__ ) || defined( __APPLE__ )
    #include <dirent.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
#endif

#if defined( WIN32 )
    #include <string>
    #include <Windows.h>
#endif

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel( wxNotebook* parent );
        void refreshSerial();
        void OnRefreshSerial( wxCommandEvent& event );
        void OnAddSwitch( wxCommandEvent& event );
        void AddSwitch();
        void OnUpdateSwitch( wxCommandEvent& event );
        void OnRemoveSwitch( wxCommandEvent& event );
        void OnSelectSwitch( wxCommandEvent& event );
        void SelectSwitch();
        void loadSwitches();

        // Topsizer
        wxBoxSizer* topSizer;

        //Picker
        wxStaticBox* switchPickerBox;
        wxStaticBoxSizer* switchPickerBoxSizer;
        wxListBox* m_switchPicker;

        // EditSizer
        wxFlexGridSizer* editSizer;

        // Edit Controls
        wxStaticText* labelName;
        wxTextCtrl* switchName;

        wxBoxSizer* refreshSizer;
        wxStaticText* labelPort;
        wxChoice* portPicker;
        wxButton* m_RefreshBtn;
        wxStaticText* labelGpio;
        wxSpinCtrl* gpioPicker;
        wxStaticText* labelDir;
        wxChoice* dirPicker;
        wxStaticText* labelManufacturer;
        wxChoice* manufacturerPicker;

        wxArrayString dirList;
        wxArrayString manufacturerList;
        wxArrayString serialArray;

        enum
        {
            ID_RefreshSerial,
            ID_AddSwitch,
            ID_UpdateSwitch,
            ID_RemoveSwitch,
            ID_SelectSwitch
        };

    private:
        DECLARE_EVENT_TABLE()
};

#endif