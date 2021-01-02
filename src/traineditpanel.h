#ifndef traineditpanel_h
#define traineditpanel_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/statbox.h"

#include "pfeditbox.h"
#include "pupeditbox.h"

class trainEditPanel : public wxPanel
{ 
    public:
        trainEditPanel(wxNotebook* parent);
        void OnChangeControler(wxCommandEvent& event);
        void RefreshPanel();

        wxBoxSizer* topSizer;
        wxStaticBox* leftBox;
        wxStaticBoxSizer* leftSizer;
        wxListBox* m_trainPicker;
        wxStaticBox* rightBox;
        wxSizer* rightSizer;
        wxRadioBox* trainKindPicker;
        wxStaticBox* m_trainEditBox;
        wxStaticBox* saveBox;
        wxSizer* saveSizer;
        wxButton* m_AddBtn;
        wxButton* m_UpdateBtn;
        wxButton* m_RemoveBtn;
        wxNotebook* panelParent;

    private:
        DECLARE_EVENT_TABLE();
};

enum
{
    ID_ADD,
    ID_REMOVE,
    ID_UPDATE,
    ID_CHANGECONTROL
};
#endif