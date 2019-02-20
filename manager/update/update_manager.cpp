//
// Created by survari on 24.12.18.
//

#include <package/package.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include <runtime.hpp>
#include <std/tstd.hpp>
#include <manager/install/installation_manager.hpp>
#include "update_manager.hpp"

void UpdateManager::updatePackage(const Package &to_update)
{
    std::cout << std::endl;

    if (IPackagesManager::isPackageInstalled(to_update))
    {
        if (Runtime::reinstall)
        {
            std::cout << "info: package " << tstd::package_to_argument(to_update) << " v" << to_update.getVersion() << " is already installed. reinstalling." << std::endl;
        }
        else
        {
            std::cout << "info: package " << tstd::package_to_argument(to_update) << " v" << to_update.getVersion() << " is already installed. skipping." << std::endl;
            return;
        }
    }

    std::cout << "[ update ] updating package " << tstd::package_to_argument(to_update) << std::endl;
    InstallationManager::installPackage(to_update, false);
}