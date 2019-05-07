//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_DESCRIPTION_HPP
#define TRIDYMITE_DESCRIPTION_HPP

#include <vector>

#include <manager/dependencies/dependency_manager.hpp>

void __argument_description(const std::vector<std::string> &parameters, CLI* cli)
{
    Package p = tstd::parse_package(cli->getParameters("d")[0]);
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
        std::cout << Translation::get("main.description_package_not_found", false) << " " << cli->getParameters("d")[0] << std::endl;
        std::cout << Translation::get("main.trying_find_online") << std::endl;

        Package package = DependencyManager::getPackagesConfig({p})[0];
        std::cout << package << std::endl;
    }
}

#endif //TRIDYMITE_HELP_HPP
