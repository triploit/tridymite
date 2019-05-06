//
// Created by survari on 05.05.19.
//

#ifndef TRIDYMITE_GET_HPP
#define TRIDYMITE_GET_HPP

#include <vector>

void __argument_get(const std::vector<std::string> &parameters, CLI* cli)
{
    Package p = tstd::parse_package(cli->getParameters("g")[0]);
    std::string destination = p.getRepoName()+"_"+p.getGitUser()+"_"+p.getServer()+".zip";

    std::cout <<  Translation::get("main.downloading", false) << " " << cli->getParameters("g")[0] << "..." << std::endl;

    if (std::ifstream(destination).is_open())
    {
        if (tstd::yn_question(Translation::get("main.file_exists_overwrite", false)))
        {
            if (tstd::download_file(tstd::create_zip_url(p), destination))
                std::cout << Translation::get("main.finished", false) << std::endl;
            else
                std::cout << Translation::get("main.package_not_found", false) << std::endl;
        }
    }
    else
    {
        if (tstd::download_file(tstd::create_zip_url(p), destination))
            std::cout << Translation::get("main.finished", false) << std::endl;
        else
            std::cout << Translation::get("main.package_not_found", false) << std::endl;
    }
}

#endif //TRIDYMITE_HELP_HPP
