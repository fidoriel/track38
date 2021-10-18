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

        wxFlexGridSizer* topSizerPfEdit = nullptr;
        wxStaticText* nameTxt = nullptr;
        wxTextCtrl* trainName = nullptr;
        wxStaticText* portTxt = nullptr;
        wxStaticText* gpioTxt = nullptr;
        wxSpinCtrl* gpioPicker = nullptr;
        wxStaticText* channelTxt = nullptr;
        wxArrayString channel;
        wxChoice* channelPicker = nullptr;
        wxStaticText* subChannelTxt = nullptr;
        wxArrayString subChannel;
        wxChoice* subChannelPicker = nullptr;
        wxStaticText* maxSpeedTxt = nullptr;
        wxSpinCtrl* maxSpeedPicker = nullptr;
        wxBoxSizer* refreshSizer = nullptr;
        wxButton* m_RefreshBtn = nullptr;
        wxConfigBase* configTrain = nullptr;
        wxConfigBase* track38ConfigTrain = nullptr;
    
    enum
    {
        ID_REFRESHSERIAL
    };
};

#endif