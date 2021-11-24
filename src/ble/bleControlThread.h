#ifndef bleControlThread_h
#define bleControlThread_h

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

#include "../poweredupp/PoweredUpp.h"

#include "wx/thread.h"

#include <memory>
#include <thread>
#include <vector>

class bleControlThread : wxThread
{
public:
    bleControlThread();
    ~bleControlThread();

    void queHub( std::string hub );
    void sendCommand();

    SimpleBLE::Adapter* adapter = nullptr;
    std::vector< SimpleBLE::Peripheral > results;
    std::vector< std::tuple< std::string, SimpleBLE::Peripheral> > hubList;
    std::vector< std::string > hubQue;

    PoweredUpp pup;

private:
    virtual ExitCode Entry() wxOVERRIDE;
};

#endif