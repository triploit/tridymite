#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

#include <std/arguments.hpp>

int main(int argc, char* argv[])
{
    Runtime::init();

    CLI cli;
    cli.init(__ARG_NAME,
            __ARG_HELP,
            __ARG_LENGTH,
            __ARG_USAGE,
            "tridymite",
            Version("0.0.1b"));

    bool working = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc)); // Parsing the given arguments, returns a boolean (false if there are errors)

    if (!working || argc == 1) // If there are errors or there is only one argument, we're printing the help page.
    {
        if (!working)
            std::cout << std::endl;

        cli.printHelp(argv[0]);
        Runtime::exit(1);
    }
    else
    {
        if (cli.argumentGiven("h")) // Printing the help page
            cli.printHelp("");

        // Setting the variables

        if (cli.argumentGiven("u"))
            Runtime::git_user = cli.getParameters("u")[0]; // Setting the standard git user

        if (cli.argumentGiven("s"))
            Runtime::git_user = cli.getParameters("s")[0]; // Setting the standard git server

        if (cli.argumentGiven(("e")))
            Runtime::verbose = true;

        if (cli.argumentGiven(("n")))
            Runtime::insecure = true;

        // Adding the packages

        if (cli.argumentGiven("i")) // Check if argument -i/--install is used
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("i")); // Get all parameters and convert them to packages
            Runtime::to_install.insert(Runtime::to_install.end(), pkgs.begin(), pkgs.end()); // Add Packages to install.
        }

        if (cli.argumentGiven("u"))
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("i"));
            Runtime::to_update.insert(Runtime::to_update.end(), pkgs.begin(), pkgs.end());
        }

        if (cli.argumentGiven("r"))
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("i"));
            Runtime::to_remove.insert(Runtime::to_remove.end(), pkgs.begin(), pkgs.end());
        }

        // Running the managers, doing installing, updating etc.


    }

    Runtime::exit(0);
}