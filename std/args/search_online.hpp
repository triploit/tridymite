//
// Created by survari on 06.05.19.
//

#ifndef TRIDYMITE_SEARCH_ONLINE_HPP
#define TRIDYMITE_SEARCH_ONLINE_HPP

#include <vector>
#include <numeric>

void __argument_search_online(const std::vector<std::string> &parameters, CLI* cli)
{
    if (parameters.size() < 2)
    {
        std::cout << Translation::get("main.so.too_few_parameters");
        Runtime::exit(1);
    }

    std::string server = parameters[0];
    std::string search = "";

    for (const std::string &s : std::vector<std::string>(parameters.begin() + 1, parameters.end()))
        search += "\"" + s + "\" ";

    if (!Runtime::config["servers"])
    {
        std::cout << Translation::get("main.so.no_git_servers");
        Runtime::exit(1);
    }

    if (!Runtime::config["servers"][server])
    {
        printf(Translation::get("main.so.server_not_defined").c_str(), server.c_str());
        Runtime::exit(1);
    }

    if (!Runtime::config["servers"][server]["servant"])
    {
        printf(Translation::get("main.so.servant_not_implemented").c_str(), server.c_str());
        std::cout << Translation::get("main.so.no_servant");

        Runtime::exit(1);
    }

    char path[FILENAME_MAX];
    getcwd(path, FILENAME_MAX);
    chdir(Runtime::tridy_dir.c_str());

    if (system((Runtime::config["servers"][server]["servant"].as<std::string>()+" "+search).c_str()) != 0)
    {
        std::cout << Translation::get("main.so.error_at_servant");
        Runtime::exit(1);
    }


    YAML::Node n = YAML::LoadFile("/tmp/tridy_tmp_YAML.yaml");
    chdir(path);

    if (n["return_code"])
    {
        switch (n["return_code"].as<int>())
        {
            case 1:
                std::cout << Translation::get("main.so.error");
                Runtime::exit(1);
                break;

            case 2:
                std::cout << Translation::get("main.so.invalid_pat");
                Runtime::exit(1);
                break;
        }
    }

    if (n["size"])
    {
        if (n["size"].as<int>() > 0)
        {
            int size = n["size"].as<int>();

            if (size > 1)
                printf(Translation::get("main.so.found_matches").c_str(), size);
            else
                printf(Translation::get("main.so.found_match").c_str(), size);
        }
        else
            std::cout << Translation::get("main.so.no_found");
    }

    if (n["packages"])
    {
        std::vector<std::string> raw_packages = n["packages"].as<std::vector<std::string>>();

        for (int i = 0; i < raw_packages.size(); i++)
        {
            std::vector<std::string> result = tstd::split(raw_packages[i], '!');

            if (result.size() < 3)
            {
                std::cout << Translation::get("main.so.incomplete_package");
                Runtime::exit(1);
            }

            Package package(tstd::parse_package(result[0]));
            Version version(result[1]);

            std::string info;
            info= std::accumulate(result.begin()+2, result.end(), info);

            std::cout << "[" << package.getGitUser() << "/" << package.getRepoName() << "] " << "v" << version << std::endl;
            std::cout << "    " << info << std::endl;
            std::cout << std::endl;
        }
    }
}

#endif //TRIDYMITE_HELP_HPP
