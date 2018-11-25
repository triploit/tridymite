#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

const std::vector<std::string> __ARG_NAME{
    "i,install",
    "r,remove",
    "u,update",
    "ua,update-all",
    "t,test",
    "l,local"
};

const std::vector<std::string> __ARG_HELP {
    "Install a package",
    "Remove a package",
    "Update a package",
    "Update all installed packages",
    "A really long message, I think more than 20 characters. I hope it'll work. Let's look around...",
    "Install a package from the source in the current directory."
};

int main(int argc, char* argv[])
{
    CLI cli;
    cli.init(__ARG_NAME, __ARG_HELP, "tridymite", Version("0.0.1a"));

    if (true) // !cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc)) || argc == 1
    {
        cli.printHelp();
        exit(1);
    }
    else
    {

    }
}