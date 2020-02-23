#ifndef TRIDYMITE_LIST_PACKAGES_HPP
#define TRIDYMITE_LIST_PACKAGES_HPP

#include <vector>

void __argument_list_packages(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<std::string> msgs; // Vector of messages
    msgs = tstd::create_list_of_packages(IPackagesManager::getInstalledPackages());

    if (msgs.size() > 0)
    {
        if (Runtime::try_local)
            std::cout << Translation::get("main.list_local_packages", false) << std::endl;
        else
            std::cout << Translation::get("main.list_packages", false) << std::endl;


        for (const std::string &s : msgs) // Printing all messages
            std::cout << s << std::endl;
    }
    else
    {
        if (Runtime::try_local)
            std::cout << Translation::get("main.no_local_packages_installed", false) << std::endl;
        else
            std::cout << Translation::get("main.no_packages_installed", false) << std::endl;
    }
}

#endif //TRIDYMITE_HELP_HPP
