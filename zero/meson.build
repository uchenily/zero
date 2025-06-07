source_files = files(
  'cmdline.cpp',
  'token.cpp',
  'lexer.cpp',
  'parser.cpp',
  'interpreter.cpp',
  'environment.cpp',
  'function.cpp',
  'vm.cpp',
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

zero = executable('zero', 'main.cpp',
  include_directories: includes,
  cpp_args: compile_args,
  # install: true,
  # install_dir: meson.project_source_root(),
  dependencies: dependencies)
