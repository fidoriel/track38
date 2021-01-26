#ifndef track38App_h
#define track38App_h

#include "wx/wx.h"
#include <string>
#include "wx/fileconf.h"
#include "wx/config.h"
#include "wx/image.h"
#include "wx/preferences.h"
#include "wx/scopedptr.h"

#include "preferences.h"

class track38App : public wxApp
{
public:
    wxString appName = "track38";
    wxString vendorName = "fidoriel";

    #ifdef __APPLE__
    wxString ini_dir = wxFileName::GetHomeDir() + "/Library/Application Support/" + appName + "/";
    #elif __linux__
    wxString ini_dir = wxFileName::GetHomeDir() + "/." + appName + "/";
    #elif __WIN32__
    wxString ini_dir = wxFileName::GetHomeDir() + "\\AppData\\Roaming\\" + appName + "\\";
    #endif

    virtual bool OnInit();
    void ShowPreferencesEditor( wxWindow* parent );

private:
    wxScopedPtr<wxPreferencesEditor> m_prefEditor;
};

// Implements track38 GetApp()
DECLARE_APP( track38App );

#endif