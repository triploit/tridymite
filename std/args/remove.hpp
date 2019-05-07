//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_REMOVE_HPP
#define TRIDYMITE_REMOVE_HPP

#include <vector>

void __argument_remove(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<Package> pkgs = tstd::parse_package_arguments(cli->getParameters("r"));
    Runtime::to_remove.insert(Runtime::to_remove.end(), pkgs.begin(), pkgs.end());
}

#endif //TRIDYMITE_HELP_HPP
