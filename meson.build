project('samurai', 'cpp',
        version: '1.0.0',
        default_options: ['cpp_std=c++17', 'prefix=/usr', 'optimization=3'])

# Global Variables
conf_data = configuration_data()
lib_src = []
lib_dep = []

# Includes
inc = [
  include_directories('include'),
  include_directories('.')
]

# Gather Files
subdir('src')
subdir('include')

# Build Projects
if not meson.is_cross_build()
  subdir('python')
  subdir('test')
endif
