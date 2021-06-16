project(
  'samurai', 'cpp',
  version: '1.0.0',
  default_options: [
    'cpp_std=c++17',
  ]
)

summary({'cpp_std': get_option('cpp_std'),
         'prefix': get_option('prefix'),
         'buildtype': get_option('buildtype'),
         'crossbuild': meson.is_cross_build(),
        }, section: 'General', bool_yn: true)

# Global Variables
conf_data = configuration_data()
lib_src = []
lib_dep = []

message(get_option('includedir'))
message(get_option('prefix'))

# Includes
inc = [
  include_directories(get_option('prefix') + '/include'),
  include_directories('include'),
  include_directories('.'),
]

# Gather Files
subdir('src')
subdir('include')

# Build Projects
if not meson.is_cross_build()
  subdir('python')
  subdir('test')
endif
