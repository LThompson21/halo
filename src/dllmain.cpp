#include <windows.h>

#include <thread>

#if defined( HALO_BUILD )

namespace halo
{

// typedef to ensure HALO_LIB can be modified in DllMain
struct HaloLib
{
    std::thread::id main_thread; // initialized in DllMain, used to ensure that all calls to core lib functions are from
                                 // the main thread
    bool is_init{ false };
};
extern HaloLib HALO_LIB;

} // namespace halo

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