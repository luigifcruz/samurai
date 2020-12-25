install_headers('samurai.h')

# Generate Configuration Header
conf_data.set('version', meson.project_version())
configure_file(input : 'config.h.in',
               output : 'config.h',
               configuration : conf_data)
