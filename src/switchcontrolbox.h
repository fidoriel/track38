#ifndef switchcontrolbox_h
#define switchcontrolbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include <unordered_map>
#include <algorithm>

#include "switch.h"
#include "switchcontrol.h"

class switchControlBox : public wxStaticBox
{ 
    public:
        switchControlBox( wxPanel* parent, int id, wxString title, wxString boxName, std::unordered_map< wxString, int > &cons );
        ~switchControlBox();

        switchControlPanel* m_switchControlPanel;
        wxStaticBoxSizer* sizer;

        wxPanel* parent;
};

#endif