cmake_minimum_required(VERSION 3.12)
project(tridymite)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cli)

add_executable(tridymite
        cli/cli.cpp
        cli/cli.hpp
        pkg/example_package.yaml
        pkg/package.sh
        pkg/package.yaml
        function.cpp
        function.hpp
        LICENSE
        main.cpp
        Makefile
        package.cpp
        package.hpp
        PKGBUILD
        ReadMe.md
        runtime.hpp
        script.cpp
        script.hpp
        version.hpp std/tstd.hpp)