#ifndef pupeditbox_h
#define pupeditbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include "wx/spinctrl.h"

class pupEditBox : public wxStaticBox
{ 
    public:
        pupEditBox(wxPanel* parent, int id, wxString title);
};

#endif