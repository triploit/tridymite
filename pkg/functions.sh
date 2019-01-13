#!/usr/bin/env bash

function test { # current directory is always the package directory
    cmake .
    make

    mkdir tridymite_dir
    cp -r pkg/conf/ tridymite_dir/
    mv tridymite tridymite_dir/tridy
}