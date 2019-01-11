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
    "l,local",
    "e,extended-output,verbose",
    "n,not-secure",
    "s,server",
    "u,user",
    "version",
    "lf,language-file",
    "lp,list-packages",
    "t,testing"
};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
    "help",
    "install",
    "remove",
    "update",
    "update_all",
    "local",
    "verbose",
    "insecure",
    "server",
    "user",
    "version",
    "language_file",
    "list_packages",
    "testing"
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
    1
};

#endif //TRIDYMITE_ARGUMENTS_HPP
