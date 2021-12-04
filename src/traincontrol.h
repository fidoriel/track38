#ifndef traincontrol_h
#define traincontrol_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include <unordered_map>
#include <algorithm>

#include "train.h"
#include "traincontrol.h"

enum
{
    ID_RefreshBmpButton
};

class trainControlPanel : public wxScrolledWindow
{ 
    public:
        trainControlPanel( wxPanel* parent, int id );
        void createControlBox();
        void loadTrains( std::unordered_map< wxString, int > &cons );

        void OnStopAll( wxCommandEvent& event );
        void StopAll();
        void BLEDisconnectAll();
        void OnButtonRefresh( wxThreadEvent& event );

        ~trainControlPanel();

        list< train* > trains;

        wxFlexGridSizer* topSizer = nullptr;
        wxButton* stopAllBtn = nullptr;
        wxPanel* parent = nullptr;
        wxConfigBase* configTrain = nullptr;
        wxConfigBase* track38ConfigTrain = nullptr;
    
    enum
    {
        ID_StopAll,
    };

    private:
        DECLARE_EVENT_TABLE()
};

#endif