#ifndef TRIDYMITE_DEPENDENCY_MANAGER_HPP
#define TRIDYMITE_DEPENDENCY_MANAGER_HPP


#include <package/package.hpp>

class DependencyManager
{
public:
    static void checkDependencies(const Package package_to_check);
    static std::vector<Package> getPackagesConfig(std::vector<Package> packages, bool show_message=true);
};


#endif //TRIDYMITE_DEPENDENCY_MANAGER_HPP
