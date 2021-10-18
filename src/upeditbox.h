#ifndef upeditbox_h
#define upeditbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/checkbox.h"
#include "wx/confbase.h"
#include "wx/dialog.h"
#include <wx/msgdlg.h>

#include "editBox.h"
#include "ble/bleConDialog.h"

class upEditBox : public editBox
{ 
    public:
        upEditBox( wxPanel* parent, int id, wxString title );
        void SelectTrain( wxString trainSel );
        void AddTrain( wxString trainName  );
        bool SaveTrain( bool overwrite = false );
        void SetTrainName( wxString name );
   
    private:
        DECLARE_EVENT_TABLE();

        void initConf();
        void OnBleMenue( wxCommandEvent& event );

        wxPanel* parentPanel = nullptr;
        wxFlexGridSizer* topSizerUpEdit = nullptr;
        wxTextCtrl* bleDevName = nullptr;
        wxStaticText* nameTxt = nullptr;
        wxTextCtrl* trainName = nullptr;
        wxStaticText* bleDevTxt = nullptr;
        wxStaticText* hubAdressTxt = nullptr;
        wxTextCtrl* hubAdress = nullptr;
        wxStaticText* channelTxt = nullptr;
        wxArrayString channel;
        wxChoice* channelPicker = nullptr;
        wxStaticText* hasTwoMotorsTxt = nullptr;
        wxCheckBox* hasTwoMotors = nullptr;
        wxStaticText* maxSpeedTxt = nullptr;
        wxSpinCtrl* maxSpeedPicker = nullptr;
        wxBoxSizer* refreshSizer = nullptr;
        wxButton* m_RefreshBtn = nullptr;

        wxConfigBase* configTrain = nullptr;
        wxConfigBase* track38ConfigTrain = nullptr;

        bleConDialog* dlg = nullptr;
    
    enum
    {
        ID_SCANBLE
    };
};

#endif