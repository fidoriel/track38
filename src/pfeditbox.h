#ifndef pfeditbox_h
#define pfeditbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include "wx/spinctrl.h"

class pfEditBox : public wxStaticBox
{ 
    public:
        pfEditBox(wxPanel* parent, int id, wxString title);

        enum
        {
            ID_ADD = 542,
            ID_REMOVE = 1666,
            ID_NEW = 512
        };
};

#endif