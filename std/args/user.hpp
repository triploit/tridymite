#ifndef TRIDYMITE_USER_HPP
#define TRIDYMITE_USER_HPP

#include <vector>

void __argument_user(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::git_user = cli->getParameters("us")[0]; // Setting the standard git user
}

#endif //TRIDYMITE_HELP_HPP
