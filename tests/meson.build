executable('test_cmdline', 'test_cmdline.cpp',
  include_directories: includes,
  cpp_args: compile_args,
  dependencies: dependencies)

executable('test_lexer', 'test_lexer.cpp',
  include_directories: includes,
  cpp_args: compile_args,
  dependencies: dependencies)

all_zero_examples = [
  'examples/hello.zero',
  'examples/return.zero',
  'examples/statement_if.zero',
  'examples/statement_for.zero',
  'examples/statement_while.zero',
  'examples/statements.zero',
  'examples/function.zero',
  'examples/native_function.zero',
  'examples/fibonacci.zero',
]

foreach example: all_zero_examples
  test_name = example.replace('.cpp', '')
  test(test_name, zero,
    args: example,
    workdir: meson.project_source_root(),
    verbose: false)
endforeach
