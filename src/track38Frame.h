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

#include "traineditpanel.h"
#include "mapeditpanel.h"
#include "controlpanel.h"
#include "preferences.h"

#if defined(WIN32)
#include <Windows.h>
#endif

class track38Frame : public wxFrame
{
public:
    track38Frame();
    ~track38Frame();

    // Event handlers
    void OnNbChangeing( wxBookCtrlEvent& event );
    void OnNbChanged( wxBookCtrlEvent& event );
    void OnSettings( wxCommandEvent& event );
    void checkDir();

    wxMenu* fileMenu = nullptr;
    wxMenu* helpMenu = nullptr;
    wxMenu* controlMenu = nullptr;

    wxNotebook* m_notebook = nullptr;

    wxMenuBar* menuBar = nullptr;
    wxBoxSizer* topSizer = nullptr;
    controlPanel* m_controlPanel = nullptr;
    trainEditPanel* m_trainEditPanel = nullptr;
    mapEditPanel* m_mapEditPanel = nullptr;

    wxFileConfig *configMain = nullptr;

    int minw = 900;
    int minh = 650;

    enum
    {
        ID_NbChanged,
        ID_VisitGithub,
        ID_Reconnect,

        ID_ImportT,
        ID_ImportMS,
        ID_ExportT,
        ID_ExportMS
    };

private:
    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );

    DECLARE_EVENT_TABLE()
};

#endif