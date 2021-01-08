# Load Python
pymod = import('python')
py = pymod.find_installation('python3')

# Compile Python Bindings
py.extension_module('samurai',
                    sources: 'index.cc',
                    include_directories: inc,
                    dependencies: [
                      dependency('pybind11'),
                    ],
                    link_with: [
                      lib_samurai,
                    ],
                    install: true,
                    install_dir: py.get_install_dir())
