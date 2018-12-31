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
    "version"
};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
    "Shows this help page",
    "Install a package",
    "Remove a package",
    "Update a package",
    "Update all installed packages",
    "Installs a package from the source in the current directory.",
    "Show extended (verbose) output.",
    "Installs packages without asking security questions.",
    "Setting the server for all package arguments.",
    "Setting the user for all package arguments.",
    "Shows the version of tridymite."
};

const std::vector<std::string> __ARG_USAGE { // Help for the arguments
    "",
    "<package> ...",
    "<package> ...",
    "<package> ...",
    "",
    "<path>",
    "",
    "",
    "<server>",
    "<file>",
    ""
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
    0
};

#endif //TRIDYMITE_ARGUMENTS_HPP
