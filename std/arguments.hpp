#ifndef TRIDYMITE_ARGUMENTS_HPP
#define TRIDYMITE_ARGUMENTS_HPP

#include <string>
#include <vector>

#include <std/args/args.hpp>

const std::vector<std::string> __ARG_NAME { // Names, two names of the same argument are written in the same string and will be splitted at a comma
    "si,search-installed",
    "so,search-online",
    "l,local",
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
    "g,get",
    "i,install",
    "u,update",
    "r,remove",
    "c,create",
    "k,keep-temp"

};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
    "search_installed",
    "search_online",
    "local",
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
    "get",
    "install",
    "update",
    "remove",
    "create",
    "keep_tmp"
};

std::vector<int> __ARG_LENGTH { // How many arguments can an argument have? -1 = endless, 0 = 0, 1 = 1, ...
    -1, // Endless arguments can be provided
    -1,
    0,
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
    1,
    -1,
    -1,
    -1,
    0,
    0
};

std::vector<void (*)(const std::vector<std::string>&, CLI* cli)> __ARG_FUNCTIONS {
    &__argument_search_installed,
    &__argument_search_online,
    &__argument_local,
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
    &__argument_get,
    &__argument_install,
    &__argument_update,
    &__argument_remove,
    &__argument_create,
    &__argument_keep_tmp
};

#endif //TRIDYMITE_ARGUMENTS_HPP
