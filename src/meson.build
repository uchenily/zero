source_files = files(
  'cmdline.cpp',
)

zero_lib = library('zero',
  source_files,
  include_directories: includes,
  cpp_args: compile_args,
  dependencies: dependencies)

zero_dep = declare_dependency(
  include_directories: includes,
  link_with: zero_lib,
  dependencies: dependencies)

dependencies += zero_dep

executable('zero', 'zero.cpp',
  include_directories: includes,
  cpp_args: compile_args,
  install: true,
  install_dir: meson.project_source_root(),
  dependencies: dependencies)
