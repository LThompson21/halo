#ifndef HALO__HALO
#define HALO__HALO

#include <string>

#if defined(_WIN32)
#if defined(HALO_BUILD)
  #define HALO_DEF __declspec(dllexport)
#else
  #define HALO_DEF __declspec(dllimport)
#endif
#else
#define HALO_DEF
#endif

namespace halo
{
  HALO_DEF std::string Version();
}

#endif //HALO__HALO