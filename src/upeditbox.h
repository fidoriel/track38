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

#include "editBox.h"

class upEditBox : public editBox
{ 
    public:
        upEditBox( wxPanel* parent, int id, wxString title );
        void OnRefreshSerial( wxCommandEvent& event );

        wxFlexGridSizer* topSizerUpEdit;
        wxStaticText* nameTxt;
        wxTextCtrl* trainName;
        wxStaticText* portTxt;
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
    
    private:
        DECLARE_EVENT_TABLE();
    
    enum
    {
        ID_REFRESHSERIAL
    };
};

#endif