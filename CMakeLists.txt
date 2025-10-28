cmake_minimum_required(VERSION 3.21)
project(halo VERSION 0.1 LANGUAGES CXX)

# Options
option(HALO_BUILD_EXAMPLES "Build halo examples" ON)
option(HALO_BUILD_TESTS    "Build halo tests"    OFF)  # hook up later if you add tests
# Build shared by default; users can override with -DBUILD_SHARED_LIBS=OFF
if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

# Library
add_library(halo
  src/halo.cpp
)
add_library(halo::halo ALIAS halo)

# Public/Private includes
target_include_directories(halo
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>           # harmless even if you don't install
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

# Compiler features
target_compile_features(halo PUBLIC cxx_std_23)

# Public/Private defines similar to your Meson logic
if(MSVC)
  set(HALO_DEF /D)
else()
  set(HALO_DEF -D)
endif()

# Always define HALO_BUILD when building the lib itself (private)
target_compile_options(halo PRIVATE ${HALO_DEF}HALO_BUILD)

# If we are on a Win-like platform and building a shared lib, add HALO_SHARED
if(WIN32 AND BUILD_SHARED_LIBS)
  target_compile_options(halo PUBLIC ${HALO_DEF}HALO_SHARED)
endif()

# Version (simple header configure; optional)
include(CMakeDependentOption)
set(HALO_VERSION_STR "${PROJECT_VERSION}")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/halo/version.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/halo/version.hpp
  @ONLY
)
target_include_directories(halo PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

# Nice default output dirs so exe & dll land together for dev runs on Windows
# (Not required, but convenient)
set_target_properties(halo PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

# -------- Examples --------
if(HALO_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# -------- Exported package for *build tree* (no install required) --------
include(CMakePackageConfigHelpers)

# If you later add more libs (e.g., halo_extra), list them here too.
set(HALO_EXPORT_TARGETS halo)

# Write targets file into the build tree
export(
  TARGETS ${HALO_EXPORT_TARGETS}
  NAMESPACE halo::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/haloTargets.cmake"
)

# Minimal Config file that references the above targets (build-tree)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/HaloConfig.cmake" [=[
# Build-tree package config for halo
include("${CMAKE_CURRENT_LIST_DIR}/haloTargets.cmake")
]=])

# (Optional) register in user's package registry so plain find_package can work locally
export(PACKAGE halo)
