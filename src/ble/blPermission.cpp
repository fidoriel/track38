#if defined( WIN32 ) || defined( LINUX )

#include "blPermission.h"


bool bleIsAutorized()
{
    return true;
}

#endif