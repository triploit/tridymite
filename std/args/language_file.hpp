#ifndef TRIDYMITE_LANGUAGE_FILE_HPP
#define TRIDYMITE_LANGUAGE_FILE_HPP

#include <vector>

void __argument_language_file(const std::vector<std::string> &parameters, CLI* cli)
{
    Translation::loadConfig(cli->getParameters("lf")[0]); // Loading the language file
}

#endif //TRIDYMITE_HELP_HPP
