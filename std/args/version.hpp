#ifndef TRIDYMITE_VERSION_HPP
#define TRIDYMITE_VERSION_HPP

#include <vector>
#include <runtime.hpp>

void __argument_version(const std::vector<std::string> &parameters, CLI* cli)
{
    std::cout << Runtime::version << std::endl;
}

#endif //TRIDYMITE_HELP_HPP
