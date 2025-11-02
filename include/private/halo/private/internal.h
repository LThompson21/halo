#pragma once

#include <windows.h>

#include <string>
#include <thread>

namespace halo
{

struct HaloLib
{
    std::thread::id main_thread; // initialized in DllMain, used to ensure that all calls to core lib functions are from
                                 // the main thread
    bool is_init{ false };
    bool is_setup{ false };
    std::string last_error{ "NONE" }; // store last error for halo::GetError()
    HINSTANCE hinst_dll;
    HINSTANCE hinst_usr;
};
extern HaloLib HALO_LIB;

} // namespace halo