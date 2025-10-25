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

# Public vs private include roots
include_pub  = include_directories('include/halo')
include_priv = include_directories('include/private')
include_gen  = include_directories('halo')

subdir('halo')  # everything inside writes into builddir/halo


# At least one source (or use a proper header-only pattern instead of a library)
srcs = [
    'src/halo.cpp', # create a tiny TU if needed
]

halo_lib = library(
    'halo', srcs,
    include_directories: [include_pub, include_priv, include_gen], # used to build the lib itself
    install: true,
    version: meson.project_version(),
)

# Install only the public headers
install_subdir('include/halo', install_dir: get_option('includedir'))

pkgconfig = import('pkgconfig')
pkgconfig.generate(
    name: 'halo',
    description: 'Halo C++ utilities',
    version: meson.project_version(),
    libraries: halo_lib,
    subdirs: 'halo', # consumers get -I${prefix}/include/halo
)

# This is what dependents will see: link to lib + public includes ONLY
halo_dep = declare_dependency(
    link_with: halo_lib,
    include_directories: include_pub,
)

# --- Safety check: private headers must not be reachable from public include path ---
# has_header takes a header *name relative to* the include dirs. We pass only the public dir.
# Use a header that lives under include/private/, e.g. include/private/no_leak.hpp -> "private/no_leak.hpp"
assert(
    not cc.has_header('private/no_leak.hpp', include_directories: include_pub),
    'Private header appears reachable via public includes! Check your install rules.'
)
