//
// Created by survari on 25.11.18.
//

#ifndef TRIDYMITE_TSTD_HPP
#define TRIDYMITE_TSTD_HPP

#include <iostream>
#include <vector>

namespace tstd
{
    std::vector<std::string> split(std::string s, char delim)
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
};


#endif //TRIDYMITE_TSTD_HPP
