#ifndef bleDialog_h
#define bleDialog_h

#include "wx/wx.h"
#include "wx/dialog.h"
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "wx/panel.h"
#include "wx/gauge.h" // Progessbar
#include "wx/progdlg.h"
#include <wx/progdlg.h>
#include "wx/thread.h"

#include "bleSearch.h"

#include <memory>
#include <string>

class bleConDialog : public wxDialog
{
public:
    bleConDialog( wxWindow *parent);
    ~bleConDialog();
    bool Create();
    void triggerScan();
    void OnGetSelection( wxCommandEvent& event );
    void OnClose( wxCommandEvent& event );
    void OnScanFinished( wxThreadEvent& event );
    void OnScanFound( wxThreadEvent& event );
    void OnOK( wxCommandEvent& event );
    void scan();
    void OnTriggerScan( wxCommandEvent& event );
    std::string getBleId( std::string tag );
    std::string getBleAdr( std::string tag );
    int selection = -1;

    bool adapterExists = false;

    bleSearch *blesearch = nullptr;
    wxWindow *parent = nullptr;

    wxSizer *topSizer = nullptr;
    wxSizer *buttonSizer = nullptr;
    wxListBox *bleDeviceList = nullptr;
    wxButton *selDevButton = nullptr;
    wxButton *rescanButton = nullptr;
    wxButton *cancelButton = nullptr;

    std::vector<SimpleBLE::Peripheral> peripheralsPtr;

    enum
    {
        ID_REFRESHSCAN,
        ID_SELECTIONLISTBOX,
        ID_CANCEL
    };

private:
    DECLARE_EVENT_TABLE()
};

#endif