//
// Created by survari on 25.11.18.
//

#ifndef TRIDYMITE_TSTD_HPP
#define TRIDYMITE_TSTD_HPP

#include <iostream>
#include <vector>

namespace tstd
{
    std::vector<std::string> split(std::string s, char delim) // split a string
    {
        std::vector<std::string> result;
        std::string tmp = "";

        for (int i = 0; i < s.size(); i++)
        {
            if (s[i] == ' ')
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

    std::string add_prefix(std::string arg) // add a prefix (- or --) to argument (like "help" -> "--help")
    {
        if (arg.size() > 2)
            return "--"+arg;
        return "-"+arg;
    }

    std::vector<std::string> get_family(std::string arg, std::vector<std::string> arg_name)
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
};


#endif //TRIDYMITE_TSTD_HPP
