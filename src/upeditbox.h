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

        wxPanel* parentPanel;
        wxFlexGridSizer* topSizerUpEdit;
        wxTextCtrl* bleDevName;
        wxStaticText* nameTxt;
        wxTextCtrl* trainName;
        wxStaticText* bleDevTxt;
        wxStaticText* hubAdressTxt;
        wxTextCtrl* hubAdress;
        wxStaticText* channelTxt;
        wxArrayString channel;
        wxChoice* channelPicker;
        wxStaticText* hasTwoMotorsTxt;
        wxCheckBox* hasTwoMotors;
        wxStaticText* maxSpeedTxt;
        wxSpinCtrl* maxSpeedPicker;
        wxBoxSizer* refreshSizer;
        wxButton* m_RefreshBtn;

        wxConfigBase* configTrain;
        wxConfigBase* track38ConfigTrain;

        bleConDialog* dlg;
    
    enum
    {
        ID_SCANBLE
    };
};

#endif