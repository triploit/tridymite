cmake_minimum_required(VERSION 3.12)
project(tridymite)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(cli)
include_directories(std)
include_directories(function)
include_directories(lexer)
include_directories(parser)
include_directories(package)
include_directories(script)

add_executable(tridymite
        main.cpp
        runtime.hpp
        version.hpp

        cli/cli.cpp
        cli/cli.hpp

        function/function.cpp
        function/function.hpp

        package/package.cpp
        package/package.hpp

        std/tstd.hpp
        std/arguments.hpp

        script/token.cpp
        script/token.hpp

        script/script.cpp
        script/script.hpp)
