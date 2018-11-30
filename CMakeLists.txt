cmake_minimum_required(VERSION 3.12)
project(tridymite)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cli)
include_directories(std)
include_directories(function)
include_directories(lexer)
include_directories(parser)
include_directories(package)
include_directories(script)

add_executable(tridymite
        cli/cli.cpp
        cli/cli.hpp
        pkg/example_package.yaml
        pkg/package.sh
        pkg/package.yaml
        function/function.cpp
        function/function.hpp
        main.cpp
        package/package.cpp
        package/package.hpp
        runtime.hpp
        script/script.cpp
        script/script.hpp
        version.hpp std/tstd.hpp std/arguments.hpp)
