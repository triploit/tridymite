#ifndef TRIDYMITE_IPACKAGESMANAGER_HPP
#define TRIDYMITE_IPACKAGESMANAGER_HPP


#include <vector>
#include <package/package.hpp>

class IPackagesManager
{
private:
    static inline std::vector<Package> installed_packages;
    static inline std::vector<Package> g_installed_packages;

public:
    static void load(std::string path, std::string gpath, bool clear_all=true);
    static const std::vector<Package> &getInstalledPackages();
    static const std::vector<Package> &getGloballyInstalledPackages();
    static const Package &getPackage(const Package &p);
    static bool isPackageInstalled(const Package &p);
    static bool isPackageInstalledNVS(const Package &p);
    static std::string getPackageFile(const Package &p);

    static void setInstalledPackages(const std::vector<Package> &packages);
};


#endif //TRIDYMITE_IPACKAGESMANAGER_HPP
