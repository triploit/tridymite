//
// Created by survari on 01.01.19.
//

#ifndef TRIDYMITE_TRANSLATION_HPP
#define TRIDYMITE_TRANSLATION_HPP

#include <iostream>
#include <yaml-cpp/yaml.h>

class Translation
{
private:
    inline static YAML::Node sentences;
    inline static YAML::Node fallback;

public:
    static std::string get(const std::string &s, const bool &nl = true, const bool &bnl = false);
    static void loadConfig(std::string path);
};

#endif //TRIDYMITE_TRANSLATION_HPP
