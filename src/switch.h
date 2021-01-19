#ifndef tswitch_h
#define tswitch_h

#include <string>
#include <list>
#include "wx/string.h"
#include "wx/config.h"
#include "wx/panel.h"
#include "wx/slider.h"
#include "wx/button.h"
#include "wx/statbox.h"
#include "wx/sizer.h"
#include "wx/msgdlg.h"
#include <wx/stattext.h>

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include "serial/serial_unix.h"
#endif

using namespace std;

class tswitch

{ 
    public:
        tswitch( wxString wxName );
        ~tswitch();
        void setPort( wxString wxControl );
        void setDir( wxString dir );
        void setGPIO( int sGPIO );

        wxString getName();
        void OnToggle( wxCommandEvent& event );
        void Straight();
        void Turn();
        void ChangePos( char dir );
        int con;

        void createControls( wxStaticBox* parent );
        void CloseCon();

        wxBoxSizer* sizer;
        wxStaticText* tswitchName;
        wxButton* straightBtn;
        wxButton* turnBtn;

        wxString name;
        string control;
        char port[128];
        char* portPoint;
        char sendSignal[32];

        int sGPIO;
        char dir;
        char sPos;

        enum
        {
            ID_tswitch
        };
};

#endif