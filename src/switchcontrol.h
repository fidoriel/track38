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

class switchControlBox : public wxStaticBox
{ 
    public:
        switchControlBox( wxPanel* parent, int id, wxString title, wxString boxName );
        void createControlBox();
        void loadswitchs( std::unordered_map< wxString, int > &cons );
        void deleteswitchs();

        ~switchControlBox();

        list< tswitch* > switchs;

        wxBoxSizer* topSizer;
        wxPanel* parent;
};

#endif