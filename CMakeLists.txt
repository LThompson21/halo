cmake_minimum_required(VERSION 3.21)

project(halo
  VERSION 0.1
  LANGUAGES CXX
)

# -------- Options --------
option(HALO_BUILD_EXAMPLES "Build halo examples" ON)
option(HALO_BUILD_TESTS    "Build halo tests"    OFF) # wire up later

# # Default to shared unless user overrides
# if(NOT DEFINED BUILD_SHARED_LIBS)
#   set(BUILD_SHARED_LIBS ON)
# endif()

# Always build with PIC so static->shared flips are painless
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -------- Library --------
# If someone tries to turn it off anyway, fail fast and loud
if(DEFINED BUILD_SHARED_LIBS AND NOT BUILD_SHARED_LIBS)
  message(FATAL_ERROR "halo is always built as a shared library (DLL). Remove BUILD_SHARED_LIBS=OFF.")
endif()

# (Optional) also lock the cache value ON so presets/tools can’t fight you
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs" FORCE)

# -------- Library --------
# Force a shared lib regardless of BUILD_SHARED_LIBS
add_library(halo SHARED
  src/halo.cpp
)

add_library(halo::halo ALIAS halo)

# Public / Private include paths
# NOTE: generated goes PUBLIC because a public header may include it
target_include_directories(halo
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
    $<INSTALL_INTERFACE:include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

# C++ standard
target_compile_features(halo PUBLIC cxx_std_23)

# Defines:
#  - HALO_BUILD: only while building this target
#  - HALO_SHARED: visible to consumers IFF this target is built shared
target_compile_definitions(halo
  PRIVATE HALO_BUILD=1
  # PUBLIC  HALO_SHARED=1
)

# Visibility (optional but nice on GCC/Clang)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(halo PRIVATE -fvisibility=hidden)
endif()


# -------- Version header --------
# cmake/version.hpp.in -> generated/halo/version.hpp
set(HALO_VERSION_STR "${PROJECT_VERSION}")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/halo/version.hpp
  @ONLY
)

# -------- Output dirs (nice for local dev) --------
set_target_properties(halo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# -------- Examples --------
if(HALO_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# -------- (Optional) Tests placeholder --------
if(HALO_BUILD_TESTS)
  enable_testing()
  # add_subdirectory(tests)
endif()

# -------- Build-tree package export --------
include(CMakePackageConfigHelpers)

set(HALO_EXPORT_TARGETS halo)

# Export the targets from the *build tree*
export(
  TARGETS ${HALO_EXPORT_TARGETS}
  NAMESPACE halo::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/haloTargets.cmake"
)

# Minimal config file for the build tree
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/HaloConfig.cmake" [=[
# Build-tree package config for halo
include("${CMAKE_CURRENT_LIST_DIR}/haloTargets.cmake")
]=])

# Let local projects do: find_package(halo) without install
export(PACKAGE halo)

# ---- Auto-copy runtime DLLs for every executable (Windows, shared builds) ----
function(halo__auto_copy_runtime_dlls)
  if(NOT WIN32 OR NOT BUILD_SHARED_LIBS)
    return()
  endif()

  # Iterate all targets and attach a post-build copy to executables.
  get_property(_allTargets GLOBAL PROPERTY TARGETS)
  foreach(t IN LISTS _allTargets)
    get_target_property(_type ${t} TYPE)
    if(_type STREQUAL "EXECUTABLE")
      # Copies all DLLs this exe depends on (including halo.dll) next to the exe.
      add_custom_command(TARGET ${t} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_RUNTIME_DLLS:${t}>
                $<TARGET_FILE_DIR:${t}>
        COMMAND_EXPAND_LISTS
      )
    endif()
  endforeach()
endfunction()

# Run after the whole project is defined so every exe is seen.
cmake_language(DEFER CALL halo__auto_copy_runtime_dlls)


# -------- (Optional) Install rules --------
# If/when you want to install:
# install(TARGETS halo
#   EXPORT haloTargets
#   RUNTIME DESTINATION bin
#   LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib
# )
# install(DIRECTORY include/ DESTINATION include)
# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/halo/version.hpp"
#         DESTINATION include/halo)
# install(EXPORT haloTargets
#   NAMESPACE halo::
#   FILE haloTargets.cmake
#   DESTINATION lib/cmake/halo
# )
# configure_package_config_file(
#   "${CMAKE_CURRENT_BINARY_DIR}/HaloConfig.cmake"
#   "${CMAKE_CURRENT_BINARY_DIR}/HaloConfig.cmake" INSTALL_DESTINATION lib/cmake/halo
# )
# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/HaloConfig.cmake"
#         DESTINATION lib/cmake/halo)
