#ifndef TRIDYMITE_VERBOSE_HPP
#define TRIDYMITE_VERBOSE_HPP

#include <runtime.hpp>
#include <cli/cli.hpp>

void __argument_verbose(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::verbose = true; // Setting the standard git user
}

#endif //TRIDYMITE_VERBOSE_HPP
