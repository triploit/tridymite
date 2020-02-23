#ifndef TRIDYMITE_NO_DEPENDENCY_CHECKING_HPP
#define TRIDYMITE_NO_DEPENDENCY_CHECKING_HPP

#include <vector>

void __argument_no_dependency_checking(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::no_dependencies = true;
}

#endif //TRIDYMITE_HELP_HPP
