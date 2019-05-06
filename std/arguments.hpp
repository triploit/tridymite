//
// Created by survari on 26.11.18.
//

#ifndef TRIDYMITE_ARGUMENTS_HPP
#define TRIDYMITE_ARGUMENTS_HPP

#include <string>
#include <vector>

#include <std/args/args.hpp>

const std::vector<std::string> __ARG_NAME { // Names, two names of the same argument are written in the same string and will be splitted at a comma
    "h,help",
    "ua,update-all",
    "p,path",
    "n,not-secure",
    "gs,git-server",
    "us,user",
    "version",
    "lf,language-file",
    "lp,list-packages",
    "d,description",
    "a,already-installed",
    "nc,no-dependency-checking",
    "f,force",
    "l,local",
    "g,get",
    "s,search",
    "i,install",
    "u,update",
    "r,remove"

};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
    "help",
    "update_all",
    "path",
    "insecure",
    "server",
    "user",
    "version",
    "language_file",
    "list_packages",
    "description",
    "already_installed",
    "no_deps",
    "force",
    "local",
    "get",
    "search",
    "install",
    "update",
    "remove"
};

std::vector<int> __ARG_LENGTH { // How many arguments can an argument have? -1 = endless, 0 = 0, 1 = 1, ...
    0,
    0,
    1,
    0,
    1,
    1,
    0,
    1,
    0,
    1,
    0,
    0,
    0,
    0,
    -1,
    1,
    -1, // Endless arguments can be provided
    -1,
    -1
};

std::vector<void (*)(const std::vector<std::string>&, CLI* cli)> __ARG_FUNCTIONS {
        &__argument_help,
        &__argument_update_all,
        &__argument_path,
        &__argument_not_secure,
        &__argument_server,
        &__argument_user,
        &__argument_version,
        &__argument_language_file,
        &__argument_list_packages,
        &__argument_description,
        &__argument_already_installed,
        &__argument_no_dependency_checking,
        &__argument_force,
        &__argument_local,
        &__argument_get,
        &__argument_search,
        &__argument_install,
        &__argument_update,
        &__argument_remove
};

#endif //TRIDYMITE_ARGUMENTS_HPP
