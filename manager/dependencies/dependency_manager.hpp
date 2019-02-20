//
// Created by survari on 12.01.19.
//

#ifndef TRIDYMITE_DEPENDENCY_MANAGER_HPP
#define TRIDYMITE_DEPENDENCY_MANAGER_HPP


#include <package/package.hpp>

class DependencyManager
{
public:
    static void checkDependencies(const Package &package_to_check);

    static std::vector<Package> getPackageConfig(std::vector<Package> packages);
};


#endif //TRIDYMITE_DEPENDENCY_MANAGER_HPP
