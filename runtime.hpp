#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include <fstream>

#include <Yaml.hpp>

int fun_to_your_computer();

#include <package.hpp>

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
    inline static Yaml::Node language_strings;

    static void init()
    {
        git_server =  "github.com";
        tridy_dir = "/usr/share/tridymite/";
        language = "english";

        struct stat info;

        if (stat(tridy_dir.c_str(), &info) != 0)
        {
            Runtime::exit(fun_to_your_computer());
        }

        tmp_dir = std::string("/tmp/tridy-"+std::to_string(getpid()));

        if (mkdir(tmp_dir.c_str(), 0777) == -1)
        {
            std::cout << "error: couldn't create tmp-directory!: " << strerror(errno) << std::endl;
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

    static bool loadLanguagePack(std::string file="")
    {
        language_strings.Clear();

        if (file.empty())
            file = "/usr/share/tridymite/conf/lang/"+language+".yaml";

        std::ifstream f;
        f.open(file);

        if (!f.is_open())
            return false;

        std::string content;
        std::string line;

        while (std::getline(f, line))
            content += "\n" + line;

        Yaml::Parse(language_strings, content);
        f.close();

        return (language_strings.Size() != 0);
    }

    static std::string getSentence(const std::string &key)
    {
        std::string s;
        std::string ret;

        try
        {
            s = language_strings[key].As<std::string>();
        }
        catch (const Yaml::Exception &e)
        {
            std::cout << "error in yaml-parsing: " << e.Type() << " exception: " << e.what() << std::endl;
        }

        if (s.empty())
        {
            std::cout << "error: language-file: couldn't get sentence \"" << key << "\"" << std::endl;
            Runtime::exit(1);
        }

        if (s.find("\\n") != std::string::npos)
        {
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] == '\\' && s[i+1] == 'n')
                {
                    ret += "\n";
                    i++;
                }
                else
                    ret += s[i];
            }
        }

        return ret;
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