# build by default
default: build

# setup build env
setup:
    meson setup --reconfigure build

# build
build:
    meson compile -C build

# run zero
run:
    ./build/src/zero

# run pre-commit
pre-commit:
    pre-commit run -a
