#pragma once

#include <string>
#include <thread>

namespace halo
{

struct HaloLib
{
    std::thread::id main_thread; // initialized in DllMain, used to ensure that all calls to core lib functions are from
                                 // the main thread
    bool is_init{ false };
    std::string last_error{ "NONE" }; // store last error for halo::GetError()
};
extern HaloLib HALO_LIB;

} // namespace halo