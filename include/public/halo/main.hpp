#pragma once

// Usage:
//   #define HALO_MAIN_NO_ARG          // optional; default is argc/argv flavor
//   #include <halo/main.hpp>
//   // ... user code ...
//   int main() { /* becomes halo::main() */ }

#if !defined( __cplusplus )
#error "halo/main.hpp requires C++"
#endif

// --- Link-enforcement marker -----------------------------------------------
// Exactly one TU must define this function (non-inline, external linkage).
// If zero TUs include this header, the /INCLUDE:-u linker option we add
// will complain. If >1 include it, you'll get multiple-definition errors.
extern "C" void halo__user_entry_present( void );
extern "C" void halo__user_entry_present( void ) {} // one TU only

// --- Public declaration of the user's real entry point ---------------------
#include "halo/setup_name.hpp" // HALO_SETUP macro
extern "C" void HALO_SETUP();  // imported from DLL
namespace halo
{
#if defined( HALO_MAIN_NO_ARG )
auto main() -> int;
#else
auto main( int argc, char **argv ) -> int;
#endif
} // namespace halo

// real main
auto main( int argc, char **argv ) -> int
{
    HALO_SETUP();

#if defined( HALO_MAIN_NO_ARG )
    ( void ) argc;
    ( void ) argv;
    return halo::main();
#else
    return halo::main( argc, argv );
#endif
}

#ifdef main
#undef main
#endif
#define main halo::main
