pymod = import('python')
py = pymod.find_installation('python3')

py.extension_module('samurai',
                    sources: 'index.cc',
                    include_directories: inc,
                    dependencies: [
                      pybind_dep,
                    ],
                    link_with: [
                      lib_samurai,
                    ])
