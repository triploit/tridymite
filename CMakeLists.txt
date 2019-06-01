cmake_minimum_required(VERSION 3.12)
project(tridymite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES "-lyaml-cpp -lcurl")

include_directories(.)
include_directories(cli)
include_directories(std)
include_directories(std/arguments/)
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

        std/tstd.cpp
        std/tstd.hpp
        std/arguments.hpp

        script/token.cpp
        script/token.hpp

        script/script.cpp
        script/script.hpp

        manager/install/installation_manager.cpp
        manager/install/installation_manager.hpp

        manager/remove/remove_manager.cpp
        manager/remove/remove_manager.hpp

        manager/update/update_manager.cpp
        manager/update/update_manager.hpp

        manager/packages/ipackage_manager.cpp
        manager/packages/ipackage_manager.hpp

        translation/translation.cpp

        manager/dependencies/dependency_manager.cpp
        manager/dependencies/dependency_manager.hpp

        manager/pretypes/pretype_manager.cpp
        manager/pretypes/pretype_manager.hpp

        pretype/pretype.cpp
        pretype/pretype.hpp

        variable/variable.cpp
        variable/variable.hpp

        std/args/args.hpp)