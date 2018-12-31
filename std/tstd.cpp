//
// Created by survari on 22.12.18.
//

#include <tstd.hpp>
#include <unistd.h>

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
            p.setName(tmp);
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

std::string tstd::create_url(Package p, std::string postfix)
{
    if (!postfix.empty())
        postfix = "/"+postfix;

    return std::string("https://"+p.getServer()+"/"+p.getGitUser()+"/"+p.getName()+postfix);
}

void tstd::dont_rush(std::string s)
{
    __useconds_t speed;

    for (char c : s)
    {
        speed = 40000;

        if (c == '.')
            speed = speed*5;

        std::cout << c;
        std::cin.sync();
        usleep(speed);
    }
}
