lib_src = files([
    'device.cc',
    'channel.cc',
])

lib_dep = [
    limesuite_dep,
]

lib_samurai = shared_library('samurai',
                             lib_src,
                             include_directories: inc,
                             dependencies: lib_dep,
                             install: true)
