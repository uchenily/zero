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
