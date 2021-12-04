#ifndef train_h
#define train_h

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

#include "ble/bleControlThread.h"
#include "poweredup/PoweredUp.h"

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include "serial/serial_unix.h"
#endif

#if defined(WIN32)
#include "serial/serial_win.h"
#endif

using namespace std;

class train

{ 
    public:
        train( wxString wxName );
        ~train();

        bleControlThread *blect = nullptr;
        wxPanel* parent = nullptr;

        void setControl( wxString wxControl );
        void setPort( wxString wxControl );
        void setMaxSpeed( wxString wxSpeed );
        void setHubAdress( wxString wxHub );
        void setHubName( wxString wxHub );
        void setUpChannel( wxString wxCh );
        void setTwoMotors( bool is );
        void setPfChannel( wxString wxCh );
        void setPfSubChannel( wxString wxSub );
        void setPfGPIO( wxString wxGPIO );
        void SetButton( wxString file );
        bool connectBLE();
        void SetCorrectPNG();

        wxString getName();
        void OnStop( wxCommandEvent& event );
        void OnChangeSpeed( wxCommandEvent& event );
        void ChangeSpeed(int newSpeed);
        void Stop();
        void CloseCon();
        int con;

        void createControls( wxPanel* parent );

        bool isConnected = false;
        int speed = 0;

        bool isPf();
        bool isUp();
        bool isRc();

        wxBoxSizer* sizer = nullptr;

        wxStaticText* trainName  = nullptr;
        wxSlider* speedSlider  = nullptr;
        wxButton* stopBtn  = nullptr;

        wxString name;
        string control;
        char port[128];
        char* portPoint;
        int maxTrainSpeed;

        //UP
        string upHubAdress;
        string upHubName;
        char upChannel;
        bool upTwoMotorsUsed;

        //PF
        int pfChannel;
        char pfSubChannel;
        int pfGpio;
        char sendSignal[32];

        enum
        {
            ID_StopTrain,
            ID_ChangeSpeed
        };
    private:
        string stopPNG = "stop.png";
        string connectPNG = "ble.png";
        string disconectPNG = "bleCancel.png";
};

#endif