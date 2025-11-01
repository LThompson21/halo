#include <windows.h>

#include <thread>

#include "halo/private/internal.h"

#if defined( HALO_BUILD )

// #include "halo/version.hpp"

WINAPI auto DllMain( HINSTANCE hinst, DWORD reason, LPVOID /*unused*/ ) -> bool
{
    if ( reason == DLL_PROCESS_ATTACH ) {
        DisableThreadLibraryCalls( hinst );
        halo::HALO_LIB.main_thread = std::this_thread::get_id();
        // MessageBoxW( nullptr, L"Halo Attached", L"HALO" HALO_VERSION, MB_OK );
    }
    if ( reason == DLL_PROCESS_DETACH ) {
        // MessageBoxW( nullptr, L"Halo Detached", L"HALO" HALO_VERSION, MB_OK );
    }
    return TRUE;
}

#endif // HALO_BUILD