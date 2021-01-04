# Generate Configuration Header
conf_data.set('version', meson.project_version())
configure_file(input : 'config.hpp.in',
               output : 'config.hpp',
               configuration : conf_data,
               install : true,
               install_dir : get_option('includedir') + '/samurai')

# Install Headers
install_subdir('samurai', install_dir : get_option('includedir'))
