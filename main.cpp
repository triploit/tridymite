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
            Version(Runtime::version),
            __ARG_FUNCTIONS);

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
        cli.runArguments();

        // Running the managers, doing installing, updating etc.

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
                    printf(Translation::get("main.package_installed_reinstall").c_str(), tstd::package_to_argument(test, true).c_str(), test.getVersion().str.c_str());
                }
                else
                {
                    printf(Translation::get("main.package_installed_skipping").c_str(), tstd::package_to_argument(test, true).c_str(), test.getVersion().str.c_str());
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

    std::cout << std::endl << Translation::get("main.all_work_done");

    if (!Runtime::to_install.empty())
    {
        std::cout << Translation::get("main.arrow", false);

        if (Runtime::to_install.size() > 1)
            printf(Translation::get("main.installed_t").c_str(), Runtime::to_install.size());
        else
            printf(Translation::get("main.installed_o").c_str(), Runtime::to_install.size());
    }

    if (!Runtime::to_update.empty())
    {
        std::cout << Translation::get("main.arrow", false);

        if (Runtime::to_update.size() > 1)
            printf(Translation::get("main.updated_t").c_str(), Runtime::to_update.size());
        else
            printf(Translation::get("main.updated_o").c_str(), Runtime::to_update.size());
    }

    if (!Runtime::to_remove.empty())
    {
        std::cout << Translation::get("main.arrow", false);

        if (Runtime::to_remove.size() > 1)
            printf(Translation::get("main.removed_t").c_str(), Runtime::to_remove.size());
        else
            printf(Translation::get("main.removed_o").c_str(), Runtime::to_remove.size());
    }

    Runtime::exit(0);
}
