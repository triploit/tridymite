//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_FORCE_HPP
#define TRIDYMITE_FORCE_HPP

#include <vector>

void __argument_force(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::force = true;
}

#endif //TRIDYMITE_HELP_HPP
