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
            printf(Translation::get("manager.install.local_reinstall").c_str(), tstd::package_to_argument(to_update).c_str(), to_update.getVersion().str.c_str());
        }
        else
        {
            printf(Translation::get("manager.install.local_skip").c_str(), tstd::package_to_argument(to_update).c_str(), to_update.getVersion().str.c_str());
            return;
        }
    }

    std::cout << "[ " << Translation::get("manager.update.updating", false) << " ] " << Translation::get("manager.update.updating_package", false) << " " << tstd::package_to_argument(to_update) << std::endl;
    InstallationManager::installPackage(to_update, false);
}