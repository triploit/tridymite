//
// Created by survari on 24.12.18.
//

#include <package/package.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include <runtime.hpp>
#include <std/tstd.hpp>
#include <manager/install/installation_manager.hpp>
#include "update_manager.hpp"

void UpdateManager::updatePackage(const Package &p)
{
    std::cout << std::endl;

    if (IPackagesManager::isPackageInstalled(p))
    {
        if (Runtime::reinstall)
        {
            std::cout << "info: package " << tstd::package_to_argument(p) << " v" << p.getVersion() << " is already installed. reinstalling." << std::endl;
        }
        else
        {
            std::cout << "info: package " << tstd::package_to_argument(p) << " v" << p.getVersion() << " is already installed. skipping." << std::endl;
            return;
        }
    }

    std::cout << "[ update ] updating package " << tstd::package_to_argument(p) << std::endl;
    InstallationManager::installPackage(p, false);
}