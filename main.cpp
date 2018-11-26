#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

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

std::vector<int> __ARG_LENGTH { // How many arguments can an argument have? -1 = endless, 0 = 0, 1 = 1, ...
    -1, // Endless arguments can be provided
    -1,
    -1,
    0,
    0,
    1
};

int main(int argc, char* argv[])
{
    CLI cli;
    cli.init(__ARG_NAME, __ARG_HELP, __ARG_LENGTH, "tridymite", Version("0.0.1a"));

    bool yes = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc));

    if (!yes || argc == 1)
    {
        if (!yes)
            std::cout << std::endl;

        cli.printHelp();
        exit(1);
    }
    else
    {
        std::vector<std::string> families;

        for (std::string s : __ARG_NAME)
        {
            std::string tmp;
            bool found = false;

            for (char c : s)
            {
                if (c == ',')
                {
                    if (!found)
                    {
                        families.push_back(tmp);
                        found = true;
                    }

                    tmp = "";
                    continue;
                }

                tmp += c;
            }

            if (!tmp.empty())
            {
                if (!found)
                {
                    families.push_back(tmp);
                    found = true;
                }

                tmp = "";
            }
        }

        for (const std::string &f : families)
        {
            std::cout << f << " : ";

            for (const std::string &s : cli.getParameters(f))
            {
                std::cout << s << " ";
            }

            std::cout << std::endl;
        }
    }
}