#include <cstdlib>
#include <iostream>

#include <halo/halo.hpp>

#define HALO_MAIN_NO_ARG
#include <halo/main.hpp>

auto main() -> int
{
    std::cout << "Hello HALO " << halo::Version() << "\n";
    if ( !halo::Init() ) { std::cout << "Init Failed: " << halo::GetError() << "\n"; }
    return EXIT_SUCCESS;
}
