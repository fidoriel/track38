#ifndef traineditpanel_h
#define traineditpanel_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/statbox.h"
#include "wx/fileconf.h"
#include "wx/config.h"

#include "pfeditbox.h"
#include "upeditbox.h"

class trainEditPanel : public wxPanel
{ 
    public:
        trainEditPanel( wxNotebook* parent );
        void OnChangeControler( wxCommandEvent& event );
        void OnAddTrain( wxCommandEvent& event );
        void OnUpdateTrain( wxCommandEvent& event );
        void OnSelectTrain( wxCommandEvent& event );
        void SelectTrain();
        void OnRemoveTrain( wxCommandEvent& event );
        void SaveTrain();
        void RefreshPanel();
        void RemoveTrain();

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
        wxConfigBase* configTrain;
        wxConfigBase* track38ConfigTrain;

    private:
        DECLARE_EVENT_TABLE();
};

enum
{
    ID_RemoveTrain,
    ID_UpdateTrain,
    ID_ChangeControl,
    ID_AddTrain,
    ID_SelectTrain
};
#endif