#ifndef TRIDYMITE_HELP_HPP
#define TRIDYMITE_HELP_HPP

#include <vector>

void __argument_help(const std::vector<std::string> &parameters, CLI* cli)
{
    cli->printHelp("");
}

#endif //TRIDYMITE_HELP_HPP
