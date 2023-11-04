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
