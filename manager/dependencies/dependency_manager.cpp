//
// Created by survari on 12.01.19.
//

#include <runtime.hpp>
#include <std/tstd.hpp>
#include "dependency_manager.hpp"

void DependencyManager::checkDependencies(const Package &p)
{
    if (Runtime::no_dependencies)
        return;

    for (Package pkg : p.getDependencies())
    {
        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &x = Runtime::to_remove[i];

            if (x.getRepoName() == pkg.getRepoName() &&
                x.getGitUser() == pkg.getGitUser() &&
                x.getServer() == pkg.getServer())
            {
                std::cout << "info: can't remove " << tstd::package_to_argument(Runtime::to_remove[i]) << ": is a dependency of: " << tstd::package_to_argument(p) << std::endl;
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
            }

            continue;
        }

        bool found = false;

        for (int x = 0; x < Runtime::to_install.size(); x++)
        {
            const Package &i = Runtime::to_install[x];

            if (i.getRepoName() == pkg.getRepoName() &&
                i.getGitUser() == pkg.getGitUser() &&
                i.getServer() == pkg.getServer())
            {
                found = true;
                Runtime::to_install.insert(Runtime::to_install.begin(), i);
                Runtime::to_install.erase(Runtime::to_install.begin()+x+1);
            }
        }

        if (!found)
        {
            for (const Package &i : IPackagesManager::getInstalledPackages())
            {
                if (i.getRepoName() == pkg.getRepoName() &&
                    i.getGitUser() == pkg.getGitUser() &&
                    i.getServer() == pkg.getServer())
                    found = true;
            }
        }

        if (!found)
        {
            std::cout << "info: added dependency: " << tstd::package_to_argument(pkg) << std::endl;
            DependencyManager::checkDependencies(pkg);
            Runtime::to_install.insert(Runtime::to_install.begin(), pkg);
        }
    }
}