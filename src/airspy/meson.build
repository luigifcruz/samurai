airspy_deps = [
  dependency('libairspy', required: false)
]

all_airspy_deps_found = true
foreach x_dep : airspy_deps
  all_airspy_deps_found = all_airspy_deps_found and x_dep.found()
endforeach

if all_airspy_deps_found
  conf_data.set('SAMURAI_AIRSPY_ENABLED', true)
  lib_dep += airspy_deps
  lib_src += files([
    'device.cc',
    'channel.cc',
  ])
else
  if get_option('AIRSPY')
    error('Airspy dependencies were not found.')
  endif
endif

sum_lst += {'Airspy One': all_airspy_deps_found}