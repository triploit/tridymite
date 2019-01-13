#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

#include <std/arguments.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include <manager/install/installation_manager.hpp>
#include <algorithm>
#include <manager/dependencies/dependency_manager.hpp>
#include <manager/remove/remove_manager.hpp>

#define _VERSION "0.0.3a"

int main(int argc, char* argv[])
{
    Runtime::init();

    if (tstd::exec("whoami") == "root\n")
    {
        std::cout << "error: never run tridymite as root!" << std::endl;
        Runtime::exit(1);
    }

    CLI cli;
    cli.init(__ARG_NAME,
            __ARG_HELP,
            __ARG_LENGTH,
            "tridymite",
            Version(_VERSION));

    bool working = cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc)); // Parsing the given arguments, returns a boolean (false if there are errors)

    if (!working || argc == 1) // If there are errors or there is only one argument, we're printing the help page.
    {
        if (!working)
            std::cout << std::endl;

        if (cli.argumentGiven("lf"))
            Translation::loadConfig(cli.getParameters("lf")[0]);

        cli.printHelp(argv[0]);
        Runtime::exit(1);
    }
    else
    {
        if (cli.argumentGiven("lf")) // Set language file
            Translation::loadConfig(cli.getParameters("lf")[0]); // Loading the language file

        if (cli.argumentGiven("h")) // Printing the help page
            cli.printHelp("");

        if (cli.argumentGiven("lp")) // List all installed packages
        {
            std::vector<std::string> msgs; // Vector of messages

            for (const Package &p : IPackagesManager::getInstalledPackages())
            {
                msgs.push_back("    - "+p.getGitUser()+" -> "+p.getRepoName()+" @ "+p.getServer()+" - v"+p.getVersion().str); // Add every message to vector
            }

            std::sort(msgs.begin(), msgs.end()); // Sort messages alphabet

            std::cout << "Packages installed: " << std::endl;
            for (const std::string &s : msgs) // Printing all messages
                std::cout << s << std::endl;
        }

        if (cli.argumentGiven("d"))
        {
            Package p = tstd::parse_package(cli.getParameters("d")[0]);

            if (IPackagesManager::isPackageInstalled(p))
                std::cout << IPackagesManager::getPackage(p) << std::endl;
            else
            {
                std::cout << "error: show description: couldn't find package " << cli.getParameters("d")[0] << std::endl;
                Runtime::exit(1);
            }
        }

        if (cli.argumentGiven("version"))
        {
            std::cout << _VERSION << std::endl;
        }

        // Setting the variables

        if (cli.argumentGiven("ua"))
        {
            Runtime::update_all = true;

            Runtime::to_update.insert(Runtime::to_update.end(),
                    IPackagesManager::getInstalledPackages().begin(),
                    IPackagesManager::getInstalledPackages().end());
        }

        if (cli.argumentGiven("us"))
            Runtime::git_user = cli.getParameters("us")[0]; // Setting the standard git user

        if (cli.argumentGiven("s"))
            Runtime::git_user = cli.getParameters("s")[0]; // Setting the standard git server

        if (cli.argumentGiven(("e")))
            Runtime::verbose = true; // Show verbose output

        if (cli.argumentGiven(("n"))) // Don't ask security questions
            Runtime::insecure = true;

        if (cli.argumentGiven("a"))
            Runtime::reinstall = true;

        if (cli.argumentGiven("nc"))
            Runtime::no_dependencies = true;

        // Adding the packages

        if (cli.argumentGiven("i")) // Check if argument -i/--install is used
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("i")); // Get all parameters and convert them to packages
            Runtime::to_install.insert(Runtime::to_install.end(), pkgs.begin(), pkgs.end()); // Add Packages to install
        }

        if (cli.argumentGiven("u") && !Runtime::update_all)
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("u"));
            Runtime::to_update.insert(Runtime::to_update.end(), pkgs.begin(), pkgs.end());
        }

        if (cli.argumentGiven("r"))
        {
            std::vector<Package> pkgs = tstd::parse_package_arguments(cli.getParameters("r"));
            Runtime::to_remove.insert(Runtime::to_remove.end(), pkgs.begin(), pkgs.end());
        }

        // Running the managers, doing installing, updating etc.

        if (cli.argumentGiven("l")) // Install local path
        {
            std::cout << "info: installing local path: " << cli.getParameters("l")[0] << std::endl;
            chdir(cli.getParameters("l")[0].c_str());
            InstallationManager::localPackage(cli.getParameters("l")[0]);
        }

        if (Runtime::to_install.size() == 0 &&
            Runtime::to_update.size() == 0 &&
            Runtime::to_remove.size() == 0)
            Runtime::exit(0);

        std::cout << "info: checking packages..." << std::endl;

        for (int i = 0; i < Runtime::to_install.size(); i++) // Checking if package is already installed - https://github.com/user/repository/raw/branch/filename
        {
            const Package &p = Runtime::to_install[i];

            std::string file = Runtime::tmp_dir+"/tmp.yaml";
            std::string url = tstd::create_url(p, "raw/master/pkg/package.yaml");

            if (!tstd::download_file(url, file))
            {
                std::cout << "error: package " << tstd::package_to_argument(p) << " not found!" << std::endl;
                Runtime::exit(1);
            }

            std::ofstream _of(file, std::ios::app);
            _of << "gituser: " << p.getGitUser() << std::endl;
            _of << "reponame: " << p.getRepoName() << std::endl;
            _of << "server: " << p.getServer() << std::endl;
            _of.close();

            Package test(YAML::LoadFile(file));

            if (IPackagesManager::isPackageInstalled(test))
            {
                if (Runtime::reinstall)
                {
                    std::cout << "info: package " << tstd::package_to_argument(p) << " v" << test.getVersion() << " is already installed. reinstalling." << std::endl;
                }
                else
                {
                    std::cout << "info: package " << tstd::package_to_argument(p) << " v" << test.getVersion() << " is already installed. skipping." << std::endl;
                    Runtime::to_install.erase(Runtime::to_install.begin()+i);
                }
            }

            Runtime::to_install[i] = test;
            std::remove(file.c_str());
        }

        std::cout << "info: searching for dependencies..." << std::endl;

        for (const Package &p : Runtime::to_install) // Check for dependencies
        {
            DependencyManager::checkDependencies(p);
        }

        for (const Package &p : IPackagesManager::getInstalledPackages()) // Check for dependencies
        {
            bool cont = false;

            for (const Package &i : Runtime::to_install) // Check for dependencies
            {
                if (i.getGitUser() == p.getGitUser() &&
                    i.getServer() == p.getServer() &&
                    i.getRepoName() == p.getRepoName())
                {
                    cont = true;
                    break;
                }
            }

            if (!cont)
                DependencyManager::checkDependencies(p);
        }

        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &p = Runtime::to_remove[i];
            bool found = false;

            for (const Package &p2 : IPackagesManager::getInstalledPackages())
            {
                if (p2.getGitUser() == p.getGitUser() &&
                    p2.getServer() == p.getServer() &&
                    p2.getRepoName() == p.getRepoName())
                {
                    found = true;
                    continue;
                }
            }

            if (!found)
            {
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
                std::cout << "info: package " << tstd::package_to_argument(p) << " is not installed and can't be removed." << std::endl;
            }
        }

        if (Runtime::to_remove.size() > 0 ||
            Runtime::to_install.size() > 0 ||
            Runtime::to_update.size() > 0)
            std::cout << std::endl;

        if (Runtime::to_remove.size() > 0)
        {
            std::cout << "info: following packages will be removed (" << Runtime::to_remove.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_remove)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_update.size() > 0)
        {
            std::cout << "info: following packages will be updated (" << Runtime::to_update.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_update)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_install.size() > 0)
        {
            std::cout << "info: following packages will be installed (" << Runtime::to_install.size() << "): " << std::endl;
            std::cout << "  => ";

            for (const Package &p : Runtime::to_install)
                std::cout << tstd::package_to_argument(p) << " ";
            std::cout << std::endl << std::endl;
        }

        if (Runtime::to_install.size() == 0 &&
            Runtime::to_update.size() == 0 &&
            Runtime::to_remove.size() == 0)
            Runtime::exit(0);

        std::cout << "do you want to continue?" << std::endl;
        std::cout << "[y/n] : ";
        std::string s;

        while (s != "y" && s != "Y" && s != "n" && s != "N")
        {
            std::getline(std::cin, s);

            if (s != "y" && s != "Y" && s != "n" && s != "N")
            {
                std::cout << "please type y or n!" << std::endl;
                std::cout << "[y/n] : ";
            }
        }

        if (s == "N" || s == "n")
            Runtime::exit(0);

        // Installing/Updating/Removing

        for (const Package &p : Runtime::to_remove) // Iterate through packages, that have to be removed
        {
            RemoveManager::uninstallPackage(p);
        }

        for (const Package &p : Runtime::to_update) // Iterate through packages, that have to be updated
        {

        }

        for (const Package &p : Runtime::to_install) // Iterate through packages, that have to be installed
        {
            InstallationManager::installPackage(p);
        }

    }

    Runtime::exit(0);
}