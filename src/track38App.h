#ifndef track38App_h
#define track38App_h

#include "wx/wx.h"
#include <string>
#include "wx/fileconf.h"
#include "wx/config.h"
#include "wx/image.h"
#include "wx/preferences.h"
#include "wx/scopedptr.h"
#include <wx/stdpaths.h>

#include "preferences.h"

class track38App : public wxApp
{
public:
    wxString appName = "track38";
    wxString vendorName = "fidoriel";

    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    #ifdef __APPLE__
    // wxString ini_dir = wxFileName::GetHomeDir() + "/Library/Application Support/" + appName + "/";
    wxString ini_dir = stdp.GetUserDataDir() + "/";
    #elif __linux__
    wxString ini_dir = stdp.GetUserDataDir() + "/";
    #elif __WIN32__
    wxString ini_dir = stdp.GetUserDataDir() + "\\";
    #endif

    #ifdef __APPLE__
    wxString ressourcePath = wxString( "track38.app/Contents/Resources/" );
    #elif defined(__linux__) || defined(__FreeBSD__)
    wxString ressourcePath = "../usr/share/icons/";
    #elif __WIN32__
    wxString ressourcePath = "resources/";
    #endif

    virtual bool OnInit();
    void ShowPreferencesEditor( wxWindow* parent );

    // Dnd veto workaround
    bool vetoDND;

private:
    wxScopedPtr<wxPreferencesEditor> m_prefEditor;
};

// Implements track38 GetApp()
DECLARE_APP( track38App );

#endif