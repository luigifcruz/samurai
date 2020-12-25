limesdr_deps = [
  dependency('LimeSuite', required: false)
]

all_limesdr_deps_found = true
foreach x_dep : limesdr_deps
  all_limesdr_deps_found = all_limesdr_deps_found and x_dep.found()
endforeach

if all_limesdr_deps_found
  conf_data.set('SAMURAI_LIMESDR_ENABLED', true)
  lib_dep += limesdr_deps
  lib_src += files([
    'device.cc',
    'channel.cc',
  ])
  message('LimeSDR Enabled: YES')
else
  if get_option('LIMESDR')
    error('LimeSDR dependencies were not found.')
  endif
  message('LimeSDR Enabled: NO')
endif
