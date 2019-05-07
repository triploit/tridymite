//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_PATH_HPP
#define TRIDYMITE_PATH_HPP

#include <vector>
#include <manager/install/installation_manager.hpp>

void __argument_path(const std::vector<std::string> &parameters, CLI* cli)
{
    std::cout << Translation::get("main.installing_local_path", false) << " " << cli->getParameters("p")[0] << std::endl;
    chdir(cli->getParameters("p")[0].c_str());
    InstallationManager::localPackage(cli->getParameters("p")[0]);
}

#endif //TRIDYMITE_HELP_HPP
