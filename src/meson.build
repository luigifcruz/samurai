subdir('base')
subdir('limesdr')
subdir('airspy')

# Build Library
lib_samurai = shared_library('samurai',
                             lib_src,
                             include_directories: inc,
                             dependencies: lib_dep,
                             install: true)
