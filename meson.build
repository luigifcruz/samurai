project('samurai', 'cpp',
        version: '1.0.0',
        default_options: ['cpp_std=c++17'])

# Global Variables
conf_data = configuration_data()
lib_src = []
lib_dep = []

# Gather Files
subdir('src')
subdir('include')

# Includes
inc = include_directories('include')

# Build Library
lib_samurai = shared_library('samurai',
                             lib_src,
                             include_directories: inc,
                             dependencies: lib_dep,
                             install: true)

# Build Dependables
#subdir('python')
subdir('test')
