//
// Created by survari on 22.12.18.
//

#include <tstd.hpp>
#include <unistd.h>
#include <curl/curl.h>

#include <iostream>
#include <memory>

std::vector<std::string> tstd::split(std::string s, char delim) // split a string by a delimiter
{
    std::vector<std::string> result;
    std::string tmp = "";

    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == delim)
        {
            result.push_back(tmp);
            tmp = "";
        }
        else
            tmp += s[i];
    }

    if (!tmp.empty())
        result.push_back(tmp);

    return result;
}

std::string tstd::add_prefix(std::string arg) // add a prefix (- or --) to argument (like "help" -> "--help"; "i" -> "-i")
{
    if (arg.size() > 2)
        return "--"+arg;
    return "-"+arg;
}

std::vector<std::string> tstd::get_family(std::string arg, std::vector<std::string> arg_name) // getting the family of an argument, e.g. "i" is contained in the family of "i,install"
{
    std::vector<std::string> family;

    for (int x = 0; x < arg_name.size(); x++)
    {
        std::string tmp;

        for (char c : arg_name[x])
        {
            if (c == ',')
            {
                family.push_back(tmp);
                tmp = "";
                continue;
            }

            tmp += c;
        }

        if (!tmp.empty())
        {
            family.push_back(tmp);
            tmp = "";
        }

        bool found = false;

        for (std::string s : family)
        {
            if (add_prefix(s) == arg)
                found = true;
        }

        if (found)
            return family;

        family.clear();
    }

    return family;
}

Package tstd::parse_package(const std::string &package)
{
    Package p;
    p.setServer(Runtime::git_server);
    p.setGitUser(Runtime::git_user);

    bool server = false;
    std::string tmp;

    for (const char &c : package)
    {
        if (server)
        {
            tmp += c;
            continue;
        }

        if (c == '@')
        {
            p.setRepoName(tmp);
            tmp = "";

            server = true;
            continue;
        }

        if (c == ':')
        {
            p.setGitUser(tmp);
            tmp = "";

            continue;
        }

        tmp += c;
    }

    if (server)
        p.setServer(tmp);
    else
        p.setRepoName(tmp);

    return p;
}

std::vector<Package> tstd::parse_package_arguments(const std::vector<std::string> &packages)
{
    std::vector<Package> to_ret;

    for (const std::string &s : packages)
    {
        to_ret.push_back(parse_package(s));
    }

    return to_ret;
}

std::string tstd::create_url(const Package &p, std::string postfix, std::string prefix)
{
    if (!postfix.empty())
        postfix = "/"+postfix;

    return std::string("https://"+prefix+p.getServer()+"/"+p.getGitUser()+"/"+p.getRepoName()+postfix);
}

std::vector<std::string> tstd::read_cursive_all_files(std::string path)
{
    std::vector<std::string> files;

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        std::string p = path+dp->d_name;
        struct stat s;

        if (stat(p.c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                std::vector<std::string> tmp = read_cursive_all_files(p+"/");
                files.insert(files.end(), tmp.begin(), tmp.end());
            }
            else if (s.st_mode & S_IFREG)
            {
                files.push_back(p);
            }
        }
    }
    closedir(dirp);

    return files;
}

bool tstd::download_file(const std::string &url, const std::string &destination)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();

    if (curl)
    {
        fp = fopen(destination.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        return ((res >= 200 && res < 300) || res == 0);
    }

    return false;
}

std::string tstd::package_to_argument(const Package &p)
{
    return std::string(p.getGitUser()+":"+p.getRepoName()+"@"+p.getServer());
}

std::string tstd::exec(const char* cmd)
{
    std::array<char, 128> buffer{};
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int tstd::cursive_file_count(const std::string &path, int count)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0)
    {
        std::cout << "error: object " << path << " not found!" << std::endl;
        Runtime::exit(1);
    }
    else if( info.st_mode & S_IFDIR)
        count++;
    else
        return 1;

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        std::string p = path+dp->d_name;
        struct stat s;

        if (stat(p.c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                count = cursive_file_count(p, count);
            }
            else if (s.st_mode & S_IFREG)
            {
                count++;
            }
        }
    }
    closedir(dirp);

    return count;
}