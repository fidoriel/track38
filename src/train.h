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
#include <wx/msgdlg.h>

using namespace std;

class train

{ 
    public:
        train( wxString wxName );
        void setControl( wxString wxControl );
        void setPort( wxString wxControl );
        void setMaxSpeed( wxString wxSpeed );
        void setHubAdress( wxString wxControl );
        void setUpChannel( wxString wxCh );
        void setTwoMotors( bool is );
        void setPfChannel( wxString wxCh );
        void setPfSubChannel( wxString wxSub );
        void setPfGPIO( wxString wxGPIO );

        wxString getName();
        void OnStop( wxCommandEvent& event );
        void OnChangeSpeed( wxScrollEvent& event );
        void ChangeSpeed(int newSpeed);
        void Stop();

        void createControls( wxStaticBox* parent );

        bool isPf();
        bool isUp();

        wxBoxSizer* sizer;

        wxStaticText* trainName;
        wxSlider* speedSlider;
        wxButton* stopBtn;

        wxString name;
        string control;
        string port;
        int maxTrainSpeed;

        //UP
        string upHubAdress;
        char upChannel;
        bool upTwoMotorsUsed;

        //PF
        int pfChannel;
        char pfSubChannel;
        int pfGpio;

        enum
        {
            ID_StopTrain,
            ID_ChangeSpeed
        };
    
    private:
        DECLARE_EVENT_TABLE()
};

#endif