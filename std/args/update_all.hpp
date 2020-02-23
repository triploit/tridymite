#ifndef TRIDYMITE_UPDATE_ALL_HPP
#define TRIDYMITE_UPDATE_ALL_HPP

#include <vector>

void __argument_update_all(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::update_all = true;

    Runtime::to_update.insert(Runtime::to_update.end(),
                              IPackagesManager::getInstalledPackages().begin(),
                              IPackagesManager::getInstalledPackages().end());
}

#endif //TRIDYMITE_HELP_HPP
