//
// Created by survari on 29.12.18.
//

#ifndef TRIDYMITE_IPACKAGESMANAGER_HPP
#define TRIDYMITE_IPACKAGESMANAGER_HPP


#include <vector>
#include <package/package.hpp>

class IPackagesManager
{
private:
    static std::vector<Package> installed_packages;

public:
    void init();

    const std::vector<Package> &getInstalledPackages();
    bool isPackageInstalled(Package p);
};


#endif //TRIDYMITE_IPACKAGESMANAGER_HPP
