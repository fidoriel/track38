#ifndef editbox_h
#define editbox_h

#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include "wx/panel.h"
#include "wx/statbox.h"
#include "wx/spinctrl.h"
#include "wx/checkbox.h"

#if defined( __linux__ ) || defined( __FreeBSD__ ) || defined( __APPLE__ )
    #include <dirent.h>
    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>
#endif

#if defined( WIN32 )
    #include <string>
    #include <Windows.h>
#endif

class editBox : public wxStaticBox
{ 
    public:
        editBox( wxPanel* parent, int id, wxString title );
        void refreshSerial();
        wxChoice* portPicker = nullptr;
        wxArrayString serialArray;
};

#endif