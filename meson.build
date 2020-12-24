project('blade', 'cpp',
        version: '1.0.0',
        default_options: ['cpp_std=c++17'])

## Dependencies
limesuite_dep = dependency('LimeSuite')
pybind_dep = dependency('pybind11')

## Includes
inc = include_directories('include')

## Modules
subdir('include')
subdir('src')
#subdir('python')
subdir('test')
