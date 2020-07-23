#ifndef TRIDYMITE_NOT_SECURE_HPP
#define TRIDYMITE_NOT_SECURE_HPP

#include <vector>

void __argument_not_secure(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::insecure_ignore_all_questions = true; // Don't press enter, just rush through everything
}

#endif //TRIDYMITE_HELP_HPP
