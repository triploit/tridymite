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

    bool working = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc)); // Parsing the given arguments, returns a boolean (false if there are errors)

    if (!working || argc == 1) // If there are errors or there is only one argument, we're printing the help page.
    {
        if (!working)
            std::cout << std::endl;

        cli.printHelp(argv[0]);
        exit(1);
    }
    else
    {
        // Doing some testing stuff...

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

        for (const std::string &f : families) // Show all arguments, that are given and their values.
        {
            if (!cli.argumentGiven(f))
                continue;

            std::cout << f << " : Given! => ( ";

            for (const std::string &s : cli.getParameters(f))
            {
                std::cout << s << " ";
            }

            std::cout << ")" << std::endl;
        }

        if (cli.argumentGiven("f"))
        {
            Script s = Script(cli.getParameters("f")[0]);
            s.go();
        }
    }
}