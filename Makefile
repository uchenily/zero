.PHONY: compile setup lint ci
install:
	meson install -C build

compile:
	meson compile -C build

setup:
	meson setup --reconfigure build

lint:
	pre-commit run -a

ci: lint

tests:
	./build/tests/test_cmdline

examples:
	./zero examples/function.zero
	./zero examples/native_function.zero
