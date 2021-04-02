#ifndef pfeditbox_h
#define pfeditbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/fileconf.h"
#include "wx/config.h"

#include "editBox.h"

class pfEditBox : public editBox
{ 
    public:
        pfEditBox( wxPanel* parent, int id, wxString title );       
        bool SaveTrain( bool overwrite = false );
        void SelectTrain( wxString trainSel );
        void AddTrain( wxString trainName  );
        void SetTrainName( wxString name );
    
    private:
        DECLARE_EVENT_TABLE();
        void initConf();
        void OnRefreshSerial( wxCommandEvent& event );

        wxFlexGridSizer* topSizerPfEdit;
        wxStaticText* nameTxt;
        wxTextCtrl* trainName;
        wxStaticText* portTxt;
        wxStaticText* gpioTxt;
        wxSpinCtrl* gpioPicker;
        wxStaticText* channelTxt;
        wxArrayString channel;
        wxChoice* channelPicker;
        wxStaticText* subChannelTxt;
        wxArrayString subChannel;
        wxChoice* subChannelPicker;
        wxStaticText* maxSpeedTxt;
        wxSpinCtrl* maxSpeedPicker;
        wxBoxSizer* refreshSizer;
        wxButton* m_RefreshBtn;
        wxConfigBase* configTrain;
        wxConfigBase* track38ConfigTrain;
    
    enum
    {
        ID_REFRESHSERIAL
    };
};

#endif