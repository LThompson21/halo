// halo/export.hpp
#pragma once

#if defined( HALO_BUILD )
#define HALO_API __declspec( dllexport )
#else
#define HALO_API __declspec( dllimport )
#endif

#define HALO_INLINE inline