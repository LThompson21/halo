#include <windows.h>
#include <winnt.h>

#include <rpcndr.h>
#include <string>
#include <thread>

#include "halo/halo.hpp"
#include "halo/version.hpp"

namespace halo
{

struct HaloLib
{
    std::thread::id main_thread; // initialized in DllMain, used to ensure that all calls to core lib functions are from
                                 // the main thread
    bool is_init{ false };
    std::string last_error{ "NONE" }; // store last error for halo::GetError()
} HALO_LIB;

} // namespace halo

auto halo::Version() -> std::string { return HALO_VERSION; }

auto halo::IsMainThread( std::thread::id *current_thread ) -> bool
{
    auto t = std::this_thread::get_id();
    if ( current_thread != nullptr ) { *current_thread = t; }
    return HALO_LIB.main_thread == t;
}

auto halo::Init() -> bool
{
    // Check for correct thread

    if ( std::thread::id tried; !halo::IsMainThread( &tried ) ) {
        const auto main_tid = std::hash<std::thread::id>{}( HALO_LIB.main_thread );
        const auto tried_tid = std::hash<std::thread::id>{}( tried );
        halo::SetErrorFmt( "Cannot initialize halo outside the main thread. Called from tid {:#x}, main tid {:#x}.",
                           tried_tid,
                           main_tid );
        return false;
    }

    if ( halo::WasInit() ) { return true; }

    return halo::HALO_LIB.is_init = true;
}

auto halo::WasInit() -> bool { return halo::HALO_LIB.is_init; }

auto halo::Terminate() -> bool
{
    // Check for correct thread

    if ( std::thread::id tried; !halo::IsMainThread( &tried ) ) {
        const auto main_tid = std::hash<std::thread::id>{}( HALO_LIB.main_thread );
        const auto tried_tid = std::hash<std::thread::id>{}( tried );
        halo::SetErrorFmt( "Cannot terminate halo outside the main thread. Called from tid {:#x}, main tid {:#x}.",
                           tried_tid,
                           main_tid );
        return false;
    }

    if ( !halo::WasInit() ) { return true; }

    // cleanup...

    HALO_LIB.is_init = false;
    return true;
}

auto halo::GetError() -> std::string { return halo::HALO_LIB.last_error; }
auto halo::FlushError() -> std::string
{
    std::string last = std::move( halo::HALO_LIB.last_error );
    halo::HALO_LIB.last_error = "NONE";
    return last;
}
auto halo::SetError( std::string error ) -> std::string
{
    std::string last = std::move( halo::HALO_LIB.last_error );
    halo::HALO_LIB.last_error = std::move( error );
    return last;
}