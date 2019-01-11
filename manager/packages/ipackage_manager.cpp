//
// Created by survari on 29.12.18.
//

#include "ipackage_manager.hpp"

#include <tstd.hpp>

#include <sys/types.h>
#include <dirent.h>

void IPackagesManager::load(std::string path) // package = /usr/share/tridymite/conf/user+name+server.xyz/
{
    std::vector<std::string> files = tstd::read_cursive_all_files(path);

    for (const std::string &s : files)
    {
        if (s[0] == '.')
            continue;

        if (s.size() >= 5 &&
            (s.substr(s.size()-5, s.size()) == ".yaml" ||
            s.substr(s.size()-4, s.size()) == ".yml"))
        {
            Package p(YAML::LoadFile(s));
            IPackagesManager::installed_packages.push_back(p);
        }
    }
}

bool IPackagesManager::isPackageInstalled(Package p)
{
    return false;
}

std::vector<Package> IPackagesManager::getInstalledPackages()
{
    return IPackagesManager::installed_packages;
}
