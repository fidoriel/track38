#ifndef bleDialog_h
#define bleDialog_h

#include "wx/wx.h"
#include "wx/dialog.h"
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "wx/panel.h"
#include <wx/gauge.h> // Progessbar
#include <wx/progdlg.h>

#include <thread>
#include <future>

#include "bleSearch.h"

class bleConDialog : public wxDialog
{
public:
    bleConDialog(wxWindow *parent);
    bool Create();
    void triggerScan( wxCommandEvent& event );
    void getSelection( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );
    void scan();
    int selection = -1;
    bool IsScanning = false;
    bool DlgIsOpen = false;

    bleSearch* blesearch;

    wxSizer* topSizer;
    wxSizer* buttonSizer;
    wxListBox* bleDeviceList;
    wxButton* selDevButton;
    wxButton* rescanButton;
    wxButton* cancelButton;

    enum
    {
        ID_REFRESHSCAN,
        ID_SELECTIONLISTBOX
    };

private:
    DECLARE_EVENT_TABLE()
};

#endif