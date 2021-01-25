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

class trainControlPanel : public wxScrolledWindow
{ 
    public:
        trainControlPanel( wxPanel* parent, int id );
        void createControlBox();
        void loadTrains( std::unordered_map< wxString, int > &cons );

        void OnStopAll( wxCommandEvent& event );
        void StopAll();

        ~trainControlPanel();

        list< train* > trains;

        wxFlexGridSizer* topSizer;
        wxButton* stopAllBtn;
        wxPanel* parent;
    
    enum
    {
        ID_StopAll
    };
};

#endif