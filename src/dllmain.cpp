#include <windows.h>

#include "halo/private/internal.h"

#if defined( HALO_BUILD )

WINAPI auto DllMain( HINSTANCE hinst, DWORD reason, LPVOID /*unused*/ ) -> bool
{
    if ( reason == DLL_PROCESS_ATTACH ) {
        DisableThreadLibraryCalls( hinst ); // disable messaging
        halo::HALO_LIB.hinst_dll = hinst;
    }
    return TRUE;
}

#endif // HALO_BUILD