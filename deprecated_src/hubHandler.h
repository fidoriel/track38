#ifndef hubHandler_h
#define hubHandler_h

#include <string>
#include <vector>

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

#include "PoweredUp.h"

#include "wx/thread.h"

enum
{

};

class hubHandler  : wxThread
{
public:
    hubHandler( std::string name );

    SimpleBLE::Adapter* adapter = nullptr;
    SimpleBLE::Peripheral* bleHub = nullptr;

    wxCriticalSection m_csCancelled;
    bool threadTerminated = false;

    std::vector<SimpleBLE::Peripheral> results;

    ~hubHandler();

private:
    //virtual ExitCode Entry() wxOVERRIDE;
};

#endif