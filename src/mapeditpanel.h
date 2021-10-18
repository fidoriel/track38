#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"
#include "wx/spinctrl.h"
#include "wx/fileconf.h"
#include "wx/statbox.h"
#include "wx/colour.h"
#include "wx/event.h"
#include "wx/dnd.h"
#include "wx/utils.h"
#include "wx/string.h"
#include "wx/menu.h"
#include <wx/arrstr.h>
#include "wx/app.h"

#include "editBox.h"
#include "cellImageRenderer.h"

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel( wxNotebook* parent );
        void OnRefreshSerial( wxCommandEvent& event );
        void saveSwitch();
        void saveSwitch( wxString name );
        void OnRemoveSwitch( wxCommandEvent& event );
        void OnSelectSwitch( wxCommandEvent& event );
        void OnDragMode( wxCommandEvent& event );
        void OnMapRemove( wxCommandEvent& event );
        void OnDragCellPicker( wxGridEvent& event );
        void OnEditSwitch( wxCommandEvent& event );
        void turn( int row, int col, bool clockwise = true);
        void OnTurnCC( wxCommandEvent& event );
        void OnTurnCW( wxCommandEvent& event );
        void RemoveMap( int row, int col);
        void SelectSwitch();
        void SelectSwitch( int row, int col );
        void loadSwitches();
        void RemoveSwitch();
        void OnLClickMap( wxGridEvent& event );
        void OnRClickMap( wxGridEvent& event );
        void LoadMapFromFile();
        void SaveMapToFile();
        void DragSwitchToMap( int row, int col, char dir );
        void initConfig();
        void OnRenameSwitch( wxCommandEvent& event );

        // Topsizer
        wxBoxSizer* topSizer;
        wxBoxSizer* bottomSizer;

        // Map
        int mapRows;
        int mapCols;
        wxGrid* map = nullptr;
        bool clickToDrag;
        wxMenu* mapMenu = nullptr;
        int eventCellRow;
        int eventCellCol;
        wxFileConfig* configMap = nullptr;
        wxConfigBase* track38ConfigMap = nullptr;

        // switch
        editBox* switchPickerBox = nullptr;
        wxStaticBoxSizer* switchPickerBoxSizer = nullptr;
        wxListBox* m_switchPicker = nullptr;

        // Symbol Picker
        wxStaticBox* mapPickerBox = nullptr;
        wxStaticBoxSizer* mapPickerBoxSizer = nullptr;
        wxGrid* pickerGrid = nullptr;

        // EditSizer
        wxFlexGridSizer* editSizer = nullptr;
        wxBoxSizer* pickerSizer = nullptr;

        // Edit Controls
        wxStaticText* labelName = nullptr;
        wxTextCtrl* switchName = nullptr;

        wxBoxSizer* refreshSizer = nullptr;
        wxStaticText* labelPort = nullptr;
        wxChoice* portPicker = nullptr;
        wxButton* m_RefreshBtn = nullptr;
        wxStaticText* labelGpio = nullptr;
        wxSpinCtrl* gpioPicker = nullptr;
        wxStaticText* labelDir = nullptr;
        wxChoice* dirPicker = nullptr;
        wxStaticText* labelManufacturer = nullptr;
        wxChoice* manufacturerPicker = nullptr;

        wxArrayString dirList;
        wxArrayString manufacturerList;
        wxString selectedSwitch;

        enum
        {
            ID_RefreshSerial,
            ID_AddSwitch,
            ID_RemoveSwitch,
            ID_SelectSwitch,
            ID_DragPicker,
            ID_Map,
            ID_DragMode,
            ID_CellRemove,
            ID_RMenuTurnCW,
            ID_RMenuTurnCC,
            ID_RMenuEdit,
            ID_RenameSwitch
        };

    private:
        DECLARE_EVENT_TABLE()
};

class mapDropTarget : public wxTextDropTarget
{
    public:
        mapDropTarget(wxGrid *grid);
        wxGrid *m_grid = nullptr;
        virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text);
};

#endif