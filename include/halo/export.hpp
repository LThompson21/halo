// halo/export.hpp
#pragma once

#if defined( _WIN32 ) || defined( __CYGWIN__ )
#if defined( HALO_SHARED )
#if defined( HALO_BUILD )
#define HALO_API __declspec( dllexport )
#else
#define HALO_API __declspec( dllimport )
#endif
#else
#define HALO_API // static lib on Windows
#endif
#else
// Optional: hide everything by default and only expose HALO_API
#if defined( HALO_VISIBILITY_DEFAULTS )
#define HALO_API __attribute__( ( visibility( "default" ) ) )
#else
#define HALO_API
#endif
#endif