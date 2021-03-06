#ifndef TRIDYMITE_INSTALLATION_MANAGER_HPP
#define TRIDYMITE_INSTALLATION_MANAGER_HPP


#include <package/package.hpp>

class InstallationManager
{
public:
    static bool installPackage(const Package &arg, bool nl=true, bool update=false);
    static bool localPackage(std::string path);
    static bool approveChanges(const std::string &prefix, const Package &package);
    static bool moveProducts(const std::string &prefix, const Package &package);
    static bool linkProducts(const std::string &prefix, const Package &package);
    static std::string downloadPackage(const std::string &prefix, const std::string &package_dir, const std::string &package_zip, const Package &arg);
};


#endif //TRIDYMITE_INSTALLATION_MANAGER_HPP
