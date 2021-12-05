#ifndef bleControlThread_h
#define bleControlThread_h

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

#include "../poweredup/PoweredUp.h"

#include "wx/thread.h"
#include "wx/msgdlg.h"

#include <memory>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <functional>

#if defined( WIN32 )
#define strcasecmp _stricmp
#endif

using namespace std;

class bleControlThread : wxThread
{
public:
    bleControlThread( string sadr, string sname );
    void setCallbackFunc(std::function<void(bool)> onConStatusChange);
    void connect();
    void RequestTermination();
    void idle();
    void sendCommand( uint8_t ary[], int len );
    ~bleControlThread();

    wxCriticalSection m_csCancelled;
    bool threadTerminated = false;
    bool hubDetected = false;

    SimpleBLE::Peripheral* hub = nullptr;

private:
    virtual ExitCode Entry() wxOVERRIDE;

    int bleCharIDX = -1;

    bool cmdInPipe = false;

    string sadr;
    string sname;
    string message;

    SimpleBLE::Adapter* adapter = nullptr;
    std::vector< SimpleBLE::Peripheral > results;

    std::function<void(bool)> onStatusChangeCallback;
    std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;

    bool connectionAttempt();
};

#endif