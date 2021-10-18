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
#include "rceditbox.h"

class trainEditPanel : public wxPanel
{ 
    public:
        trainEditPanel( wxNotebook* parent );
        void OnChangeControler( wxCommandEvent& event );
        wxString GetTrainControl( wxString trainSel );
        void OnSelectTrain( wxCommandEvent& event );
        void SelectTrain();
        void initConf();
        void RefreshPanel();
        void OnNewTrain( wxCommandEvent& event );
        void OnRemoveTrain( wxCommandEvent& event );
        void OnRenameTrain( wxCommandEvent& event );
        void SaveTrain();

        wxBoxSizer* topSizer = nullptr;
        wxStaticBox* leftBox = nullptr;
        wxStaticBoxSizer* leftSizer = nullptr;
        wxListBox* m_trainPicker = nullptr;
        wxStaticBox* rightBox = nullptr;
        wxSizer* rightSizer = nullptr;
        wxRadioBox* trainKindPicker = nullptr;
        wxStaticBox* m_trainEditBox = nullptr;

        wxBoxSizer* saveSizer = nullptr;
        wxButton* m_NewBtn = nullptr;
        wxButton* m_RenameBtn = nullptr;
        wxButton* m_RemoveBtn = nullptr;

        wxNotebook* panelParent = nullptr;
        wxConfigBase* configTrain = nullptr;
        wxConfigBase* track38ConfigTrain = nullptr;

        int lastSel = 0;

    private:
        DECLARE_EVENT_TABLE();
};

enum
{
    ID_RemoveTrain,
    ID_ChangeControl,
    ID_NewTrain,
    ID_SelectTrain,
    ID_RenameTrain
};
#endif