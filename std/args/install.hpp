//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_INSTALL_HPP
#define TRIDYMITE_INSTALL_HPP

#include <vector>

void __argument_install(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<Package> pkgs = tstd::parse_package_arguments(cli->getParameters("i")); // Get all parameters and convert them to packages
    Runtime::to_install.insert(Runtime::to_install.end(), pkgs.begin(), pkgs.end()); // Add Packages to install
}

#endif //TRIDYMITE_HELP_HPP
