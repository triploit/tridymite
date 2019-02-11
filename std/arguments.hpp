//
// Created by survari on 26.11.18.
//

#ifndef TRIDYMITE_ARGUMENTS_HPP
#define TRIDYMITE_ARGUMENTS_HPP

#include <string>
#include <vector>

const std::vector<std::string> __ARG_NAME { // Names, two names of the same argument are written in the same string and will be splitted at a comma
    "h,help",
    "i,install",
    "r,remove",
    "u,update",
    "ua,update-all",
    "p,path",
    "e,extended-output,verbose",
    "n,not-secure",
    "s,server",
    "us,user",
    "version",
    "lf,language-file",
    "lp,list-packages",
    "d,description",
    "a,already-installed",
    "nc,no-dependency-checking",
    "f,force",
    "t,testing",
    "l"
};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
    "help",
    "install",
    "remove",
    "update",
    "update_all",
    "path",
    "verbose",
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
    "testing",
    "local"
};

std::vector<int> __ARG_LENGTH { // How many arguments can an argument have? -1 = endless, 0 = 0, 1 = 1, ...
    0,
    -1, // Endless arguments can be provided
    -1,
    -1,
    0,
    1,
    0,
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
    0
};

#endif //TRIDYMITE_ARGUMENTS_HPP
