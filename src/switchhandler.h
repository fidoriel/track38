#ifndef switchhandler_h
#define switchhandler_h

#include <wx/fileconf.h>
#include <unordered_map>
#include <algorithm>

#include "switch.h"

class switchHandler
{

    public:
        switchHandler();
        void loadswitchs( std::unordered_map< wxString, int > &cons );

        ~switchHandler();

        list< tswitch* > switches;


        wxFileConfig* configMap = nullptr;
        wxConfigBase* track38ConfigMap = nullptr;
};

#endif