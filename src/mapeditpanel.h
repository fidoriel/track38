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
        void OnAddSwitch( wxCommandEvent& event );
        void AddSwitch();
        void OnUpdateSwitch( wxCommandEvent& event );
        void OnRemoveSwitch( wxCommandEvent& event );
        void OnSelectSwitch( wxCommandEvent& event );
        void OnDragMode( wxCommandEvent& event );
        void OnMapRemove( wxCommandEvent& event );
        void OnDragCellPicker( wxGridEvent& event );
        void OnEditSwitch( wxCommandEvent& event );
        void turn( int row, int col, bool clockwise = true);
        void OnTurnCC( wxCommandEvent& event );
        void OnTurnCW( wxCommandEvent& event );
        void SelectSwitch();
        void loadSwitches();
        void OnLClickMap( wxGridEvent& event );
        void OnRClickMap( wxGridEvent& event );
        void LoadMapFromFile();
        void SaveMapToFile();

        // Topsizer
        wxBoxSizer* topSizer;
        wxBoxSizer* bottomSizer;

        // Map
        int mapRows;
        int mapCols;
        wxGrid* map;
        bool clickToDrag;
        wxMenu* mapMenu;
        int eventCellRow;
        int eventCellCol;

        // switch
        editBox* switchPickerBox;
        wxStaticBoxSizer* switchPickerBoxSizer;
        wxListBox* m_switchPicker;

        // Symbol Picker

        wxStaticBox* mapPickerBox;
        wxStaticBoxSizer* mapPickerBoxSizer;
        wxGrid* pickerGrid;

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
            ID_SelectSwitch,
            ID_DragPicker,
            ID_Map,
            ID_DragMode,
            ID_CellRemove,
            ID_RMenuTurnCW,
            ID_RMenuTurnCC,
            ID_RMenuEdit
        };

    private:
        DECLARE_EVENT_TABLE()
};

class mapDropTarget : public wxTextDropTarget
{
    public:
        mapDropTarget(wxGrid *grid);
        wxGrid *m_grid;
        virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text);
};

#endif