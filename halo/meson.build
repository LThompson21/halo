conf = configuration_data()
conf.set_quoted('HALO_VERSION', 'v'+meson.project_version())
ver = meson.project_version().split('.')
conf.set('HALO_VERSION_MAJOR', ver.get(0, '0'))
conf.set('HALO_VERSION_MINOR', ver.get(1, '0'))
conf.set('HALO_VERSION_PATCH', ver.get(2, '0'))

configure_file(
  output: 'version.hpp',  # => builddir/halo/version.hpp
  configuration: conf,
  install_dir: join_paths(get_option('includedir'), 'halo')  # => ${prefix}/include/halo/version.hpp
)
