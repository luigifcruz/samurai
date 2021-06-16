subdir('base')

sum_lst = {}

# SDRs
subdir('limesdr')
subdir('airspy')
subdir('airspyhf')

summary(sum_lst, section: 'Supported SDRs', bool_yn: true)

# Build Library
lib_samurai = library('samurai',
                      lib_src,
                      include_directories: inc,
                      dependencies: lib_dep,
                      install: true)

libsamurai_dep = declare_dependency(
    include_directories: inc,
    dependencies: lib_dep,
    link_with: lib_samurai
)

pkg = import('pkgconfig')
pkg.generate(
  lib_samurai,
  libraries: lib_dep,
)
