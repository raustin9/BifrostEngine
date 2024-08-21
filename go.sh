#!/usr/bin/env bash

# Utility functions
pexec() { >&2 printf exec; >&2 printf ' %q' "$@"; >&2 printf '\n'; exec "${@:?pexec: missing command}"; }

go-build() {
    cmake -B build .
    cmake --build build
}

go-run() {
    ./build/bifrost_editor
}

"go-$@"
