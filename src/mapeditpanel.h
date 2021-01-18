#ifndef mapeditpanel_h
#define mapeditpanel_h

#include "wx/wx.h"
#include "wx/notebook.h"
#include "wx/grid.h"

class mapEditPanel : public wxPanel
{ 
    public:
        mapEditPanel( wxNotebook* parent );


        wxBoxSizer* topSizer;
        wxBoxSizer* mapSizer;
        wxBoxSizer* switchSizer;

        wxGrid* map;
        wxGrid* picker;

        wxStaticBox* switchPickerBox;
        wxStaticBoxSizer* switchPickerBoxSizer;
        wxListBox* m_switchPicker;

        wxStaticBox* editBox;
        wxStaticBoxSizer* editBoxSizer;
        wxListBox* m_editPicker;

        int mapX;
        int mapY;

    private:
    DECLARE_EVENT_TABLE()
};

#endif