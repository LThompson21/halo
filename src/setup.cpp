
#include <windows.h>

#include <thread>

#include "halo/export.hpp"
#include "halo/private/internal.h"
#include "halo/setup_name.hpp" // defines HALO_SETUP_SYM

extern "C" HALO_API void HALO_SETUP();
extern "C" HALO_API void HALO_SETUP()
{
    halo::HALO_LIB.is_setup = true;
    halo::HALO_LIB.hinst_usr = GetModuleHandle( nullptr );
    halo::HALO_LIB.main_thread = std::this_thread::get_id();
}
