cbuffer_exec = executable('test_cbuffer', 'cbuffer.cc',
                       include_directories: inc,
                       link_with: lib_samurai)

test('Circular Buffer', cbuffer_exec)

lib_exec = executable('test_lib', 'radio.cc',
                       include_directories: inc,
                       link_with: lib_samurai)

test('Radio Library', lib_exec)

