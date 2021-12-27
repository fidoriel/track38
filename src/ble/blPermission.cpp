#if defined( WIN32 ) || defined( __linux__ )

#include "blPermission.h"


bool bleIsAutorized()
{
    return true;
}

void askForPermission()
{
}

#endif