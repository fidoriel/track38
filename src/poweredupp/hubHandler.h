#ifndef hubHandler_h
#define hubHandler_h

#include <string>
#include <vector>

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

#include "PoweredUpp.h"

using namespace std;

class hubHandler
{
private:
    /* data */
public:
    hubHandler(string uuid, string name);

    SimpleBLE::Adapter* adapter;
    SimpleBLE::Peripheral* bleHub;

    ~hubHandler();
};

#endif