#include <cstdlib>
#include <iostream>

#include <halo/halo.hpp>

auto main() -> int
{
    std::cout << "Hello HALO " << halo::Version() << "\n";
    return EXIT_SUCCESS;
}
