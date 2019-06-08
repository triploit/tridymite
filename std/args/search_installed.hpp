//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_SEARCH_HPP
#define TRIDYMITE_SEARCH_HPP

#include <vector>

void __argument_search_installed(const std::vector<std::string> &parameters, CLI* cli)
{
    std::string term = cli->getParameters("si")[0];
    std::vector<std::string> strings;
    std::vector<Package> packages;
    std::string inf = "";

    for (const Package &p : IPackagesManager::getInstalledPackages())
    {
        inf = p.getVersion().ToString() + " " +
              p.getServer() + " " +
              p.getGitUser() + " " +
              p.getRepoName() + " " +
              p.getName() + " " +
              p.getDescription() + " " +
              p.getInformation();

        for (const Package &dep : p.getDependencies())
        {
            inf += "\n" + dep.getVersion().ToString() + " " +
                   dep.getServer() + " " +
                   dep.getGitUser() + " " +
                   dep.getRepoName() + " " +
                   dep.getName() + " " +
                   dep.getDescription() + " " +
                   dep.getInformation();
        }

        std::transform(inf.begin(), inf.end(), inf.begin(), ::tolower);

        if (inf.find(term) != std::string::npos)
            packages.push_back(p);
    }

    strings = tstd::create_list_of_packages(packages);
    for (const std::string &s : strings)
    {
        std::cout << s << std::endl;
    }
}

#endif //TRIDYMITE_HELP_HPP
