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
        trainControlBox( wxPanel* parent, int id, wxString title );
        train* trains;
        wxButton* button;
    
    enum
    {
        ID
    };
};

#endif