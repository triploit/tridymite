//
// Created by survari on 29.12.18.
//

#include "ipackage_manager.hpp"

#include <tstd.hpp>

#include <sys/types.h>
#include <dirent.h>

void IPackagesManager::load(std::string path, bool clear_all) // package = /usr/share/tridymite/conf/user+name+server.xyz/
{
    if (clear_all)
        IPackagesManager::installed_packages.clear();
    std::vector<std::string> files = tstd::read_cursive_all_files(path);

    for (const std::string &file : files)
    {
        if (file[0] == '.')
            continue;

        if (file.size() >= 5 &&
            (file.substr(file.size()-5, file.size()) == ".yaml" ||
            file.substr(file.size()-4, file.size()) == ".yml"))
        {
            Package p(YAML::LoadFile(file), file);
            IPackagesManager::installed_packages.push_back(p);
        }
    }
}

bool IPackagesManager::isPackageInstalled(const Package &p)
{
    for (const Package &installed_package : IPackagesManager::installed_packages)
    {
        if (p == installed_package)
            return true;
    }

    return false;
}

const std::vector<Package> &IPackagesManager::getInstalledPackages()
{
    return IPackagesManager::installed_packages;
}

const Package& IPackagesManager::getPackage(Package &p)
{
    for (const Package &installed_package : IPackagesManager::installed_packages)
    {
        if (p.getGitUser() == installed_package.getGitUser() &&
            p.getRepoName() == installed_package.getRepoName() &&
            p.getServer() == installed_package.getServer())
            return installed_package;
    }

    return p;
}

std::string IPackagesManager::getPackageFile(const Package &p)
{
    return std::__cxx11::string();
}

void IPackagesManager::setInstalledPackages(const std::vector<Package> &packages)
{
    IPackagesManager::installed_packages.clear();
    IPackagesManager::installed_packages = packages;
}
