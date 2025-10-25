project(
  'halo', 'cpp',
  version: '0.1',
  default_options: [
    'cpp_std=c++23',
    'warning_level=3',
    'b_sanitize=none',
    'buildtype=debugoptimized',
  ],
)

cc = meson.get_compiler('cpp')

# Public vs private include roots (for building)
include_pub  = include_directories('include')          # so <halo/...> resolves
include_priv = include_directories('include/private')  # build-time only

# Bring in the subdir that creates version.hpp and exports halo_gen_inc
subdir('halo')  # defines halo_gen_inc

# Sources
srcs = [
  'src/halo.cpp',
  # ...
]

halo_lib = library(
  'halo', srcs,
  include_directories: [include_pub, include_priv, halo_gen_inc],  # build sees public, private & generated
  install: true,
  version: meson.project_version(),
)

# Install only the *public* headers (include/halo/**)
install_subdir('include/halo', install_dir: get_option('includedir'))

# pkg-config: add -I${includedir} so #include <halo/...> works for consumers
pkgconfig = import('pkgconfig')
pkgconfig.generate(
  name: 'halo',
  description: 'Halo C++ utilities',
  version: meson.project_version(),
  libraries: halo_lib,
  extra_cflags: ['-I${includedir}'],   # key bit: consumers get base include dir
)

# What dependents get (no private or build dirs!)
halo_dep = declare_dependency(
  link_with: halo_lib,
  include_directories: include_pub,   # exports only ${includedir}
)

# (Optional) sanity: private headers are *not installed*. That’s what protects them.
# During the build, public include root points at source 'include/', so has_header() here
# would see private/* — that’s normal at build time, not what consumers see post-install.
