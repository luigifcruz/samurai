subdir('base')

# Radios
subdir('limesdr')
subdir('airspy')
subdir('airspyhf')

# Build Library
lib_samurai = library('samurai',
                      lib_src,
                      include_directories: inc,
                      dependencies: lib_dep,
                      install: true)
