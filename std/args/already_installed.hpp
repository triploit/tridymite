#ifndef TRIDYMITE_ALREADY_INSTALLED_HPP
#define TRIDYMITE_ALREADY_INSTALLED_HPP

#include <vector>

void __argument_already_installed(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::reinstall = true;
}

#endif //TRIDYMITE_HELP_HPP
