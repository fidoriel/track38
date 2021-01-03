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

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    #include <dirent.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
#endif

#if defined(WIN32)
    #include <string>
    #include <Windows.h>
#endif

class pfEditBox : public wxStaticBox
{ 
    public:
        pfEditBox(wxPanel* parent, int id, wxString title);
        void OnRefreshSerial(wxCommandEvent& event);
        void refreshSerial();

        wxFlexGridSizer* topSizerPfEdit;
        wxStaticText* nameTxt;
        wxTextCtrl* trainName;
        wxArrayString serialArray;
        wxStaticText* portTxt;
        wxChoice* portPicker;
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
    
    private:
        DECLARE_EVENT_TABLE();
    
    enum
    {
        ID_REFRESHSERIAL
    };
};

#endif