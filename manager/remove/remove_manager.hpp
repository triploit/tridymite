//
// Created by survari on 24.12.18.
//

#ifndef TRIDYMITE_REMOVE_MANAGER_HPP
#define TRIDYMITE_REMOVE_MANAGER_HPP


#include <string>
#include <package/package.hpp>

class RemoveManager
{
public:
    static bool unlinkProducts(const std::string &prefix, const Package &package);
    static bool removeProducts(const std::string &prefix, const Package &package);
    static void uninstallPackage(const Package &p);
};


#endif //TRIDYMITE_REMOVE_MANAGER_HPP
