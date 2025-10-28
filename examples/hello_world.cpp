#include <cstdlib>
#include <iostream>

#include <halo/halo.hpp>

int main()
{
   std::cout << "Hello " << halo::Version();
   return EXIT_SUCCESS;
}