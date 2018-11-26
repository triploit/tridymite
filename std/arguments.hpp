//
// Created by survari on 26.11.18.
//

#ifndef TRIDYMITE_ARGUMENTS_HPP
#define TRIDYMITE_ARGUMENTS_HPP

#include <string>
#include <vector>

const std::vector<std::string> __ARG_NAME{ // Names, two names of the same argument are written in the same string and will be splitted at a comma
        "i,install",
        "r,remove",
        "u,update",
        "ua,update-all",
        "t,test",
        "l,local"
};

const std::vector<std::string> __ARG_HELP { // Help for the arguments
        "Install a package",
        "Remove a package",
        "Update a package",
        "Update all installed packages",
        "A really long message, I think more than 20 characters. I hope it'll work. Let's look around...",
        "Install a package from the source in the current directory."
};

const std::vector<std::string> __ARG_USAGE { // Help for the arguments
        "<package> ...",
        "<package> ...",
        "<package> ...",
        "",
        "",
        "<path>"
};

std::vector<int> __ARG_LENGTH { // How many arguments can an argument have? -1 = endless, 0 = 0, 1 = 1, ...
        -1, // Endless arguments can be provided
        -1,
        -1,
        0,
        0,
        1
};

#endif //TRIDYMITE_ARGUMENTS_HPP
