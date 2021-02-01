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
        wxString GetTrainControl( wxString trainSel );
        void OnSelectTrain( wxCommandEvent& event );
        void SelectTrain();
        void initConf();
        void RefreshPanel();
        void OnNewTrain( wxCommandEvent& event );
        void OnRemoveTrain( wxCommandEvent& event );
        void OnRenameTrain( wxCommandEvent& event );
        void SaveTrain();

        wxBoxSizer* topSizer;
        wxStaticBox* leftBox;
        wxStaticBoxSizer* leftSizer;
        wxListBox* m_trainPicker;
        wxStaticBox* rightBox;
        wxSizer* rightSizer;
        wxRadioBox* trainKindPicker;
        wxStaticBox* m_trainEditBox;

        wxBoxSizer* saveSizer;
        wxButton* m_NewBtn;
        wxButton* m_RenameBtn;
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
    ID_ChangeControl,
    ID_NewTrain,
    ID_SelectTrain,
    ID_RenameTrain
};
#endif