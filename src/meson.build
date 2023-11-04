source_files = files(
  'hello.cpp'
)

executable('zero', source_files,
  include_directories: includes,
  cpp_args: compile_args,
  install: true,
  install_dir: meson.project_source_root(),
  dependencies: dependencies)
