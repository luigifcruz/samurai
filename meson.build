project('samurai', 'cpp',
        version: '1.0.0',
        default_options: ['cpp_std=c++17', 'prefix=/usr', 'optimization=3'])

# Global Variables
conf_data = configuration_data()
lib_src = []
lib_dep = []

# Load Python
pymod = import('python')
py = pymod.find_installation('python3')

# Load Radios
subdir('src/limesdr')
subdir('src/airspy')

# Includes
inc = [
  include_directories('include'),
  include_directories('.')
]

# Gather Files
subdir('include')
subdir('src')

# Build Projects
subdir('python')
subdir('test')
