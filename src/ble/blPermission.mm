#if defined( __APPLE__ )

#include "blPermission.h"

#import <CoreBluetooth/CoreBluetooth.h>
#import <Foundation/Foundation.h>


bool bleIsAutorized()
{

    // https://developer.apple.com/documentation/corebluetooth/cbmanagerauthorization/allowedalways
    if (CBManager.authorization == CBManagerAuthorizationAllowedAlways){
        return true;
    }
    return false;
}

#endif
