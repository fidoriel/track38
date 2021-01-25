#ifndef traincontrolbox_h
#define traincontrolbox_h

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

class trainControlBox : public wxStaticBox
{ 
    public:
        trainControlBox( wxPanel* parent, int id, wxString title, wxString boxName, std::unordered_map< wxString, int > &cons );
        ~trainControlBox();

        wxStaticBoxSizer* sizer;
        trainControlPanel* m_trainControlPanel;

        wxPanel* parent;
};

#endif