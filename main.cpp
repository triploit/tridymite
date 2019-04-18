#include <iostream>
#include <algorithm>
#include <string>
#include <algorithm>

#include "runtime.hpp"
#include "cli/cli.hpp"

#include <std/arguments.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include <manager/install/installation_manager.hpp>
#include <manager/dependencies/dependency_manager.hpp>
#include <manager/remove/remove_manager.hpp>
#include <manager/update/update_manager.hpp>

#define _VERSION "1.0.0a"

int main(int argc, char* argv[])
{
    Runtime::init();

    if (tstd::exec("whoami") == "root\n")
    {
        std::cout << Translation::get("main.never_root", false) << std::endl;
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
        if (cli.argumentGiven("l"))
        {
            struct stat info;
            std::string path = std::string(getenv("HOME"))+"/.local/";

            if(stat(path.c_str(), &info) != 0)
            {
                std::cout << Translation::get("main.no_local_module", false) << std::endl;
                Runtime::exit(1);
            }

            Runtime::try_local = true;
            Runtime::tridy_dir = std::string(getenv("HOME")) + "/.local/share/tridymite";

            Runtime::reloadManagers();
        }

        if (cli.argumentGiven("lf")) // Set language file
            Translation::loadConfig(cli.getParameters("lf")[0]); // Loading the language file

        if (cli.argumentGiven("h")) // Printing the help page
            cli.printHelp("");

        if (cli.argumentGiven("lp")) // List all installed packages
        {
            std::vector<std::string> msgs; // Vector of messages
            msgs = tstd::create_list_of_packages(IPackagesManager::getInstalledPackages());

            if (msgs.size() > 0)
            {
                if (Runtime::try_local)
                    std::cout << Translation::get("main.list_local_packages", false) << std::endl;
                else
                    std::cout << Translation::get("main.list_packages", false) << std::endl;


                for (const std::string &s : msgs) // Printing all messages
                    std::cout << s << std::endl;
            }
            else
            {
                if (Runtime::try_local)
                    std::cout << Translation::get("main.no_local_packages_installed", false) << std::endl;
                else
                    std::cout << Translation::get("main.no_packages_installed", false) << std::endl;
            }
        }

        if (cli.argumentGiven("g"))
        {
            Package p = tstd::parse_package(cli.getParameters("g")[0]);
            std::string destination = p.getRepoName()+"_"+p.getGitUser()+"_"+p.getServer()+".zip";

            std::cout <<  Translation::get("main.downloading", false) << " " << cli.getParameters("g")[0] << "..." << std::endl;

            if (std::ifstream(destination).is_open())
            {
                if (tstd::yn_question(Translation::get("main.file_exists_overwrite", false)))
                {
                    if (tstd::download_file(tstd::create_zip_url(p), destination))
                        std::cout << Translation::get("main.finished", false) << std::endl;
                    else
                        std::cout << Translation::get("main.package_not_found", false) << std::endl;
                }
            }
            else
            {
                if (tstd::download_file(tstd::create_zip_url(p), destination))
                    std::cout << Translation::get("main.finished", false) << std::endl;
                else
                    std::cout << Translation::get("main.package_not_found", false) << std::endl;
            }
        }

        if (cli.argumentGiven("s"))
        {
            std::vector<std::string> strings = tstd::create_list_of_packages(IPackagesManager::getInstalledPackages());
            std::string term = cli.getParameters("s")[0];

            for (const std::string &s : strings)
            {
                std::string copy_s = tstd::trim(s);
                std::string copy_t = tstd::trim(term);

                std::transform(copy_s.begin(), copy_s.end(), copy_s.begin(), ::tolower);
                std::transform(copy_t.begin(), copy_t.end(), copy_t.begin(), ::tolower);

                for (char c : copy_s)
                    copy_s = tstd::replace(copy_s, " ", "");

                for (char c : copy_t)
                    copy_t = tstd::replace(copy_t, " ", "");

                if (s.find(term) != std::string::npos || copy_s.find(term) != std::string::npos)
                    std::cout << s << std::endl;
            }
        }

        if (cli.argumentGiven("d"))
        {
            Package p = tstd::parse_package(cli.getParameters("d")[0]);
            bool found = false;

            for (const Package &pkg : IPackagesManager::getInstalledPackages())
            {
                if (pkg.getRepoName() == p.getRepoName() &&
                    pkg.getServer() == p.getServer() &&
                    pkg.getGitUser() == p.getGitUser())
                        found = true;
            }

            if (found)
                std::cout << IPackagesManager::getPackage(p) << std::endl;
            else
            {
                std::cout << Translation::get("main.description_package_not_found", false) << " " << cli.getParameters("d")[0] << std::endl;
                std::cout << Translation::get("main.trying_find_online") << std::endl;

                Package package = DependencyManager::getPackagesConfig({p})[0];
                std::cout << package << std::endl;
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

        if (cli.argumentGiven("gs"))
            Runtime::git_server = cli.getParameters("gs")[0]; // Setting the standard git server

        if (cli.argumentGiven(("e")))
            Runtime::verbose = true; // Show verbose output

        if (cli.argumentGiven(("n"))) // Don't ask security questions
            Runtime::insecure = true;

        if (cli.argumentGiven("a"))
            Runtime::reinstall = true;

        if (cli.argumentGiven("f"))
            Runtime::force = true;

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

        if (cli.argumentGiven("p")) // Install local path
        {
            std::cout << Translation::get("main.installing_local_path", false) << " " << cli.getParameters("p")[0] << std::endl;
            chdir(cli.getParameters("p")[0].c_str());
            InstallationManager::localPackage(cli.getParameters("p")[0]);
        }

        if (Runtime::to_install.empty() &&
            Runtime::to_update.empty() &&
            Runtime::to_remove.empty())
            Runtime::exit(0);

        Runtime::to_install = DependencyManager::getPackagesConfig(Runtime::to_install);
        Runtime::to_update = DependencyManager::getPackagesConfig(Runtime::to_update);

        if (Runtime::update_all)
        {
            for (const Package &installed_package : IPackagesManager::getInstalledPackages())
            {
                for (int i = 0; i < Runtime::to_update.size(); i++)
                {
                    if (Runtime::to_update[i].getName() == installed_package.getName() &&
                        Runtime::to_update[i].getServer() == installed_package.getServer() &&
                        Runtime::to_update[i].getRepoName() == installed_package.getRepoName())
                    {
                        if (Runtime::to_update[i].getVersion() <= installed_package.getVersion())
                        {
                            if (Runtime::reinstall)
                            {
                                printf(Translation::get("main.package_up_to_date_reinstall").c_str(), tstd::package_to_argument(installed_package).c_str(), installed_package.getVersion().str.c_str());
                            }
                            else
                            {
                                printf(Translation::get("main.package_up_to_date_skipping").c_str(), tstd::package_to_argument(installed_package).c_str(), installed_package.getVersion().str.c_str());
                                Runtime::to_update.erase(Runtime::to_update.begin()+i);
                                i--;
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &to_remove = Runtime::to_remove[i];
            bool found = false;

            for (const Package &p2 : IPackagesManager::getInstalledPackages())
            {
                if (p2.getGitUser() == to_remove.getGitUser() &&
                    p2.getServer() == to_remove.getServer() &&
                    p2.getRepoName() == to_remove.getRepoName())
                {
                    found = true;
                }
            }

            if (!found)
            {
                printf(Translation::get("main.not_installed_cant_removed").c_str(), tstd::package_to_argument(to_remove).c_str());
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
                i--;
            }
        }

        if (Runtime::to_install.size() > 0 || 
            Runtime::to_remove.size() > 0 || 
            Runtime::to_update.size() > 0)
            std::cout << Translation::get("main.checking_packages", false) << std::endl;
        else
            Runtime::exit(0);


        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            Runtime::to_remove[i] = IPackagesManager::getPackage(Runtime::to_remove[i]);
        }

        for (int i = 0; i < Runtime::to_install.size(); i++)
        {
            const Package &test = Runtime::to_install[i];

            if (IPackagesManager::isPackageInstalled(test))
            {
                if (Runtime::reinstall)
                {
                    printf(Translation::get("main.package_installed_reinstall").c_str(), tstd::package_to_argument(test).c_str(), test.getVersion().str.c_str());
                }
                else
                {
                    printf(Translation::get("main.package_installed_skipping").c_str(), tstd::package_to_argument(test).c_str(), test.getVersion().str.c_str());
                    Runtime::to_install.erase(Runtime::to_install.begin()+i);
                    i--;
                }
            }
        }

        if (!Runtime::update_all)
        {
            for (int i = 0; i < Runtime::to_update.size(); i++)
            {
                const Package &test = Runtime::to_update[i];

                if (IPackagesManager::isPackageInstalled(test))
                {
                    if (Runtime::reinstall)
                    {
                        printf(Translation::get("main.package_up_to_date_reinstall").c_str(), tstd::package_to_argument(test).c_str(), test.getVersion().str.c_str());
                    }
                    else
                    {
                        printf(Translation::get("main.package_up_to_date_skipping").c_str(), tstd::package_to_argument(test).c_str(), test.getVersion().str.c_str());
                        Runtime::to_update.erase(Runtime::to_update.begin()+i);
                        i--;
                    }
                }
            }
        }

        if (Runtime::to_update.size() > 0)
            Runtime::update = true;

        if (Runtime::to_install.size() > 0 || 
            Runtime::to_remove.size() > 0 || 
            Runtime::to_update.size() > 0)
            std::cout << Translation::get("main.searching_dependencies", false) << std::endl;

        for (const Package &p : Runtime::to_install) // Check for dependencies
        {
            DependencyManager::checkDependencies(p);
        }

        for (const Package &p : IPackagesManager::getInstalledPackages()) // Check for dependencies
        {
            bool cont = false;

            for (const Package &package_to_install : Runtime::to_install) // Check for dependencies
            {
                if (package_to_install.getGitUser() == p.getGitUser() &&
                    package_to_install.getServer() == p.getServer() &&
                    package_to_install.getRepoName() == p.getRepoName())
                {
                    cont = true;
                    break;
                }
            }

            if (!cont)
                DependencyManager::checkDependencies(p);
        }

        if (Runtime::to_remove.size() > 0 ||
            Runtime::to_install.size() > 0 ||
            Runtime::to_update.size() > 0)
            std::cout << std::endl;

        if (Runtime::to_remove.size() > 0)
        {
            std::cout << Translation::get("main.following_removed", false) << " (" << Runtime::to_remove.size() << "): " << std::endl;
            std::vector<std::string> msgs = tstd::create_list_of_packages(Runtime::to_remove);

            for (const std::string &msg : msgs)
                std::cout << msg << std::endl;  
            
            std::cout << std::endl;
        }

        if (Runtime::to_update.size() > 0)
        {
            std::cout << Translation::get("main.following_updated", false) << " (" << Runtime::to_update.size() << "): " << std::endl;
            std::vector<std::string> msgs = tstd::create_list_of_packages(Runtime::to_update);

            for (const std::string &msg : msgs)
                std::cout << msg << std::endl;  
            
            std::cout << std::endl;
        }

        if (Runtime::to_install.size() > 0)
        {
            std::cout << Translation::get("main.following_installed", false) << " (" << Runtime::to_install.size() << "): " << std::endl;
            std::vector<std::string> msgs = tstd::create_list_of_packages(Runtime::to_install);

            for (const std::string &msg : msgs)
                std::cout << msg << std::endl;  
            
            std::cout << std::endl;
        }

        if (Runtime::to_install.size() == 0 &&
            Runtime::to_update.size() == 0 &&
            Runtime::to_remove.size() == 0)
            Runtime::exit(0);

        if (!tstd::yn_question(Translation::get("general.continue_question", false)))
            Runtime::exit(0);

        // Installing/Updating/Removing

        for (const Package &remove_me : Runtime::to_remove) // Iterate through packages, that have to be removed
        {
            RemoveManager::uninstallPackage(remove_me);
        }

        for (const Package &update_me : Runtime::to_update) // Iterate through packages, that have to be updated
        {
            UpdateManager::updatePackage(update_me);
        }

        for (const Package &install_me : Runtime::to_install) // Iterate through packages, that have to be installed
        {
            InstallationManager::installPackage(install_me);
        }

    }

    Runtime::exit(0);
}