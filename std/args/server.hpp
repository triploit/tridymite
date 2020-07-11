#ifndef TRIDYMITE_SERVER_HPP
#define TRIDYMITE_SERVER_HPP

#include <vector>

void __argument_server(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::git_server = cli->getParameters("gs")[0]; // Setting the standard git server
}

#endif //TRIDYMITE_HELP_HPP
