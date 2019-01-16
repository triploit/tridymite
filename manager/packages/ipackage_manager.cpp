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

bool IPackagesManager::isPackageInstalled(const Package &p)
{
    for (const Package &tp : IPackagesManager::installed_packages)
    {
        Package p1 = p;
        Package p2 = tp;

        if (p1 == p2)
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
    for (const Package &tp : IPackagesManager::installed_packages)
    {
        if (p.getGitUser() == tp.getGitUser() &&
            p.getRepoName() == tp.getRepoName() &&
            p.getServer() == tp.getServer())
            return tp;
    }

    return Package();
}