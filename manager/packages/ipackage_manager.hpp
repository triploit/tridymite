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
    static inline std::vector<Package> installed_packages;

public:
    static void load(std::string path);
    static std::vector<Package> getInstalledPackages();
    static bool isPackageInstalled(Package p);
};


#endif //TRIDYMITE_IPACKAGESMANAGER_HPP
