#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "wx/spinctrl.h"
#include "wx/fileconf.h"

#include "editBox.h"

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel( wxNotebook* parent );
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
        editBox* switchPickerBox;
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