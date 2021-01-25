#ifndef switchcontrol_h
#define switchcontrol_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include <unordered_map>
#include <algorithm>

#include "switch.h"

class switchControlPanel : public wxScrolledWindow
{ 
    public:
        switchControlPanel( wxPanel* parent, int id );
        void createControlBox();
        void loadswitchs( std::unordered_map< wxString, int > &cons );

        ~switchControlPanel();

        list< tswitch* > switches;

        wxBoxSizer* topSizer;
        wxPanel* parent;
};

#endif