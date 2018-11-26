#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

#include "std/arguments.hpp"

int main(int argc, char* argv[])
{
    CLI cli;
    cli.init(__ARG_NAME,
            __ARG_HELP,
            __ARG_LENGTH,
            __ARG_USAGE,
            "tridymite",
            Version("0.0.1a"));

    bool yes = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc));

    if (!yes || argc == 1)
    {
        if (!yes)
            std::cout << std::endl;

        cli.printHelp(argv[0]);
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