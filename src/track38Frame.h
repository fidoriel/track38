#ifndef track38Frame_h
#define track38Frame_h

#include "wx/wx.h"
#include <string>
#include "wx/fileconf.h"
#include "wx/config.h"
#include "wx/image.h"
#include "wx/preferences.h"
#include "wx/scopedptr.h"
#include "wx/artprov.h"
#include <wx/stdpaths.h>
#include <wx/filefn.h> 
#include <wx/dir.h>

#include "../icons/AppIcon.xpm"

#include "track38App.h"
#include "traineditpanel.h"
#include "mapeditpanel.h"
#include "controlpanel.h"
#include "preferences.h"

class track38Frame : public wxFrame
{
public:
    track38Frame();
    ~track38Frame();

    // Event handlers
    void OnNbChangeing( wxBookCtrlEvent& event );
    void OnNbChanged( wxBookCtrlEvent& event );
    void OnSettings( wxCommandEvent& event );
    void Settings();

    wxMenu* fileMenu;
    wxMenu* helpMenu;
    wxMenu* controlMenu;

    wxNotebook* m_notebook;

    wxMenuBar* menuBar;
    wxBoxSizer* topSizer;
    controlPanel* m_controlPanel;
    trainEditPanel* m_trainEditPanel;
    mapEditPanel* m_mapEditPanel;

    int minw = 900;
    int minh = 650;

    enum
    {
        ID_NbChanged,
        ID_VisitGithub,
        ID_Reconnect
    };

private:
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()
};

#endif