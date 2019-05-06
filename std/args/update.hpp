//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_UPDATE_HPP
#define TRIDYMITE_UPDATE_HPP

#include <vector>

void __argument_update(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<Package> pkgs = tstd::parse_package_arguments(cli->getParameters("u"));
    Runtime::to_update.insert(Runtime::to_update.end(), pkgs.begin(), pkgs.end());
}

#endif //TRIDYMITE_HELP_HPP
