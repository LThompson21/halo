#pragma once

#include <format>
#include <string>
#include <thread>

#include "halo/export.hpp"

namespace halo
{

// Returns the current version of halo, see <halo/version.hpp>
HALO_API auto Version() -> std::string;

// Returns true if called from the main thread.
// Considered the main if it loaded the Halo DLL
HALO_API auto IsMainThread( std::thread::id *current_thread = nullptr ) -> bool;

// Setup necessary state for applications.
// Must be called from the Main thread
HALO_API auto Init() -> bool;
// Get whether halo is already init
HALO_API auto WasInit() -> bool;
// Terminate the lib. Must be called from the main thread
HALO_API auto Terminate() -> bool;

// Get the last error found by halo
HALO_API auto GetError() -> std::string;
// Set the last error to NONE
HALO_API auto FlushError() -> std::string;
// Set a plain custom error
HALO_API auto SetError( std::string error ) -> std::string;
// Set a formatted custom error
template <class... Args> inline auto SetErrorFmt( std::string_view fmt, Args &&...args ) -> std::string
{
    return SetError( std::vformat( fmt, std::make_format_args( std::forward<Args>( args )... ) ) );
}

} // namespace halo