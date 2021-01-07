# C++ Circular Buffer
cpp_cbuffer_exec = executable('test_cbuffer', 'cbuffer.cc',
                              include_directories: inc,
                              link_with: lib_samurai)

test('C++ Circular Buffer', cpp_cbuffer_exec)

# C++ SDR Library
cpp_lib_exec = executable('test_lib', 'lib.cc',
                          include_directories: inc,
                          link_with: lib_samurai)

test('C++ SDR Library', cpp_lib_exec, is_parallel: false)

# Python SDR Library
python_lib_exec = find_program('lib.py')
test('Python SDR Library', python_lib_exec, is_parallel: false)
