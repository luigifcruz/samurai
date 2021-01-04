lib_dep += [
  dependency('threads')
]
lib_src += files([
  'cbuffer.cc',
])

# Build Library
lib_samurai = shared_library('samurai',
                             lib_src,
                             include_directories: inc,
                             dependencies: lib_dep,
                             install: true)
