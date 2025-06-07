project('zero', 'cpp',
  version: '0.0.1',
  default_options: [
    'buildtype=debug',
    'default_library=static',
    'cpp_std=c++17',
    'warning_level=3'],
  meson_version: '>=1.0.0')

includes = include_directories('.')

compile_args = []

fmt_dep = dependency('fmt', version: '>=11.2.0')
dependencies = []
dependencies += fmt_dep

subdir('zero')
subdir('tests')
