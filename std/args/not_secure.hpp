//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_NOT_SECURE_HPP
#define TRIDYMITE_NOT_SECURE_HPP

#include <vector>

void __argument_not_secure(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::insecure = true; // Don't ask security questions and just install/update/remove everything given
}

#endif //TRIDYMITE_HELP_HPP
