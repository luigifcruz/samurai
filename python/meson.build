pymod = import('python')
py = pymod.find_installation('python')

pybind_dep = dependency('pybind11')

py.extension_module('samurai',
                    sources: 'index.cc',
                    include_directories: inc,
                    dependencies: [
                      pybind_dep,
                    ],
                    link_with: [
                      lib_samurai,
                    ],
                    install: true,
                    install_dir: py.get_install_dir())
