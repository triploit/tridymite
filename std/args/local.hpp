#ifndef TRIDYMITE_LOCAL_HPP
#define TRIDYMITE_LOCAL_HPP

#include <vector>

void __argument_local(const std::vector<std::string> &parameters, CLI* cli)
{
    struct stat info;
    std::string path = std::string(getenv("HOME"))+"/.local/";

    if(stat(path.c_str(), &info) != 0)
    {
        std::cout << Translation::get("main.no_local_module", false) << std::endl;
        Runtime::exit(1);
    }

    Runtime::try_local = true;
    Runtime::tridy_dir = std::string(getenv("HOME")) + "/.local/share/tridymite";

    Runtime::reloadManagers();
}

#endif //TRIDYMITE_HELP_HPP
