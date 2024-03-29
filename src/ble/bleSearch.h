#ifndef bleSearch_h
#define bleSearch_h

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

#include "wx/thread.h"
#include "wx/listbox.h"
#include "wx/msgdlg.h"
#include "wx/window.h"
#include "wx/dialog.h"

#include <memory>
#include <thread>

enum
{
    FINISHED_BLE_ID,
    ITEM_LIST_APPENDED_ID
};

class bleSearch : wxThread
{
public:
    bleSearch( wxDialog* parent, wxListBox* bleDeviceList );
    int scan();
    void startScan();
    bool CancelationReqested();
    void RequestTermination();
    ~bleSearch();

    wxCriticalSection m_csCancelled;
    bool threadTerminated = false;

    wxListBox* resultBox = nullptr;
    wxDialog* parent = nullptr;

    SimpleBLE::Adapter* adapter = nullptr;
    std::vector<SimpleBLE::Peripheral> results;

private:
    virtual ExitCode Entry() wxOVERRIDE;
};

#endif