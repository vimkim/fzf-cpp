configure:
    cmake -S . -B build

build:
    cmake --build build

configure-build: configure build
