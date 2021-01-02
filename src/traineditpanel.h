#ifndef traineditpanel_h
#define traineditpanel_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/statbox.h"

#include "pfeditbox.h"

class trainEditPanel : public wxPanel
{ 
    public:
        trainEditPanel(wxNotebook* parent);

        enum
        {
            ID_ADD = 901,
            ID_REMOVE = 902,
            ID_UPDATE = 903
        };
};

#endif