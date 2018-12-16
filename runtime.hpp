#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <cstring>
#include <unistd.h>

#include "package.hpp"

class Runtime
{
public:
    inline static std::vector<Package> to_install;
    inline static std::vector<Package> to_remove;
    inline static std::vector<Package> to_update;

    inline static std::vector<std::string> files_to_clean;
    inline static std::vector<std::string> directories_to_clean;

    inline static std::string tmp_dir;

    static void init()
    {
        tmp_dir = std::string("/tmp/tridy-"+std::to_string(getpid()));

        if (mkdir(tmp_dir.c_str(), 0777) == -1)
        {
            std::cout << "error: coudln't create tmp-directory: " << strerror(errno) << std::endl;
            Runtime::exit(1);
        }

        Runtime::directories_to_clean.push_back(tmp_dir);
    }

    static bool cleanFiles()
    {
        bool success = true;

        for (const std::string &file : Runtime::files_to_clean)
        {
            if(std::remove(file.c_str()) != 0)
            {
                std::cout << "error: couldn't delete file: " << file << std::endl;
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
                std::cout << "error: couldn't delete directory: " << file << std::endl;
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
            std::cout << "error: error at cleaning up temporary files or directories." << std::endl;
        }

        if (i != 0)
            std::cout << "exiting with code " << i << std::endl;

        std::exit(i);
    }
};

#endif