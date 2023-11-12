.PHONY: compile setup lint ci
compile:
	meson compile -C build

setup:
	meson setup --reconfigure build

install:
	meson install -C build

lint:
	pre-commit run -a

ci: lint

tests:
	./build/tests/test_cmdline

examples:
	./build/src/zero examples/function.zero
	./build/src/zero examples/native_function.zero
