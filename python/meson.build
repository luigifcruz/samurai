pymod = import('python')
py = pymod.find_installation('python3')

pybind_dep = dependency('pybind11')

py.extension_module('samurai',
                    sources: 'index.cc',
                    include_directories: inc,
                    dependencies: [
                      pybind_dep,
                    ],
                    link_with: [
                      lib_samurai,
                    ])
