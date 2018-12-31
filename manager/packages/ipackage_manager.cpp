//
// Created by survari on 29.12.18.
//

#include "ipackage_manager.hpp"

void IPackagesManager::init()
{

}

bool IPackagesManager::isPackageInstalled(Package p)
{
    return false;
}

const std::vector<Package> &IPackagesManager::getInstalledPackages()
{
    std::vector<Package> v(1);
    return v;
}
