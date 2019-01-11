#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include <fstream>

#include <yaml-cpp/yaml.h>
#include <package.hpp>
#include <manager/packages/ipackage_manager.hpp>
#include "translation/translation.hpp"

class Runtime
{
public:
    inline static std::vector<Package> to_install;
    inline static std::vector<Package> to_remove;
    inline static std::vector<Package> to_update;

    inline static std::vector<std::string> files_to_clean;
    inline static std::vector<std::string> directories_to_clean;

    inline static std::string tmp_dir;
    inline static std::string tridy_dir;

    inline static std::string git_server;
    inline static std::string git_user;

    inline static bool verbose;
    inline static bool insecure;

    inline static std::string language;

    static void init()
    {
        git_server =  "github.com";
        tridy_dir = "/usr/share/tridymite/";
        language = "english";

        struct stat info;
        if (stat(tridy_dir.c_str(), &info) != 0)
        {
            printf(Translation::get("runtime.not_installed").c_str());
            Runtime::exit(1);
        }

        if (!std::ifstream(tridy_dir+"conf/config.yaml").is_open())
        {
            std::cout << "error: tridymite's config file wasn't found!" << std::endl;
            Runtime::exit(1);
        }

        language = YAML::LoadFile(tridy_dir+"conf/config.yaml")["language"].as<std::string>();
        tmp_dir = std::string("/tmp/tridy-"+std::to_string(getpid()));

        if (mkdir(tmp_dir.c_str(), 0777) == -1)
        {
            std::cout << Translation::get("tmp_permission_error") << strerror(errno) << std::endl;
            Runtime::exit(1);
        }

        Runtime::directories_to_clean.push_back(tmp_dir);
        Translation::loadConfig(tridy_dir+"conf/lang/"+language+".yaml");


        // Loading managers

        IPackagesManager::load(tridy_dir+"conf/packages/");
    }

    static bool cleanFiles()
    {
        bool success = true;

        for (const std::string &file : Runtime::files_to_clean)
        {
            if(std::remove(file.c_str()) != 0)
            {
                printf(Translation::get("runtime.clean_files.error").c_str(), file.c_str());
                success = false;
            }
        }

        return success;
    }

    static bool clearDirectories()
    {
        bool success = true;

        for (const std::string &file : Runtime::directories_to_clean)
        {
            if(rmdir(file.c_str()) != 0)
            {
                printf(Translation::get("runtime.clean_files.error").c_str(), file.c_str());
                success = false;
            }
        }

        return success;
    }

    static void exit(int i)
    {
        if (!Runtime::cleanFiles() ||
            !Runtime::clearDirectories())
        {
            std::cout << Translation::get("runtime.clear_up_all_tmps");
        }

        if (i != 0)
            printf(Translation::get("runtime.exit").c_str(), i);

        std::exit(i);
    }
};

#endif