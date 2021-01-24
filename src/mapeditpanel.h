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
        void OnDragCellPicker( wxGridEvent& event );
        void OnDragCellMap( wxGridEvent& event );
        void SelectSwitch();
        void loadSwitches();
        void OnDClickMap( wxGridEvent& event );
        void OnRClickMap( wxGridEvent& event );

        // Topsizer
        wxBoxSizer* topSizer;
        wxBoxSizer* bottomSizer;

        // Map
        wxGrid* map;

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
            ID_Map
        };

    private:
        DECLARE_EVENT_TABLE()
};

class mapDropTarget : public wxTextDropTarget
{
    public:
        mapDropTarget(wxGrid *grid)
        {
            m_grid = grid;
        }
        wxGrid *m_grid;
        virtual bool OnDropText(wxCoord x, wxCoord y, const wxString& text)
        {
            wxGridCellCoords coordinates = m_grid->XYToCell( x, y );

            int row = coordinates.GetRow() + m_grid->GetFirstFullyVisibleRow();
	        int col = coordinates.GetCol() + m_grid->GetFirstFullyVisibleColumn();
	        // m_grid->SetCellValue(row,col, text);
            m_grid->SetCellRenderer( row, col, new cellImageRenderer( text ) );

            m_grid->ForceRefresh();
            // wxMessageBox( wxString::Format( wxT( "R %i \n C %i"), row, col ) );
            return true;
        }
};

#endif