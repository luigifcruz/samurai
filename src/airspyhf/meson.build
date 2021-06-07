airspyhf_deps = [
  dependency('libairspyhf', required: false)
]

all_airspyhf_deps_found = true
foreach x_dep : airspyhf_deps
  all_airspyhf_deps_found = all_airspyhf_deps_found and x_dep.found()
endforeach

if all_airspyhf_deps_found
  conf_data.set('SAMURAI_AIRSPYHF_ENABLED', true)
  lib_dep += airspyhf_deps
  lib_src += files([
    'device.cc',
    'channel.cc',
  ])
else
  if get_option('AIRSPYHF')
    error('Airspy HF dependencies were not found.')
  endif
endif

sum_lst += {'Airspy HF': all_airspyhf_deps_found}