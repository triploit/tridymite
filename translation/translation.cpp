//
// Created by survari on 01.01.19.
//

#include <iostream>
#include <yaml-cpp/yaml.h>

#include <runtime.hpp>

std::string Translation::get(const std::string &s, const bool &nl, const bool &bnl)
{
    if (sentences[s])
    {
        std::string str = std::string(sentences[s].as<std::string>());

        if (nl)
            str = str + "\n";
        if (bnl)
            str = "\n" + str;

        return str;
    }
    else
    {
        if (!fallback[s])
        {
            std::cout << "error: fatal error in tridymite: sentences \"" << s << "\" doesn't exist in fallback!" << std::endl;
            Runtime::exit(1);
        }
        else
        {
            std::string str = std::string(fallback[s].as<std::string>());

            if (nl)
                str = str + "\n";
            if (bnl)
                str = "\n" + str;

            return ("[?] "+str);
        }
    }

    return "<NOT FOUND>";
}

void Translation::loadConfig(std::string path)
{
    fallback = YAML::Load(R"V0G0N(
error: true
)V0G0N");

    if (std::ifstream(path).is_open())
    {
        sentences = YAML::LoadFile(path);
    }
    else
    {
        std::cout << "warning: language file " << path << " not found!" << std::endl;
        std::cout << "Using fallback..." << std::endl;
    }
}

