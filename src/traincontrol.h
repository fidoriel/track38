#ifndef traincontrol_h
#define traincontrol_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"

#include "train.h"

class trainControlBox : public wxStaticBox
{ 
    public:
        trainControlBox( wxPanel* parent, int id, wxString title, wxString boxName );
        void createControlBox();
        void loadTrains();
        void RefreshPanel();

        void OnStopAll( wxCommandEvent& event );
        void StopAll();

        ~trainControlBox();

        list<train*> trains;

        wxFlexGridSizer* topSizer;
        wxButton* stopAllBtn;
        wxPanel* parent;
    
    enum
    {
        ID_StopAll
    };
};

#endif