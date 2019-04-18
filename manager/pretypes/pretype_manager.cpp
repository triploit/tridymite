//
// Created by survari on 16.04.19.
//

#include <std/tstd.hpp>
#include "pretype_manager.hpp"


void PreTypeManager::load(std::string path)
{
    std::vector<std::string> files = tstd::read_cursive_all_files(path);

    for (const std::string &file : files)
    {
        std::string name = tstd::split(file, '/')[tstd::split(file, '/').size()-1];

        if (name[0] == '.')
            continue;

        PreTypeManager::pretypes.push_back(PreType(file, name));
    }
}

bool PreTypeManager::existsPreType(const std::string &name)
{
    for (const PreType &p : pretypes)
    {
        if (p.getName() == name)
            return true;
    }

    return false;
}

const std::vector<PreType> &PreTypeManager::getPreTypes()
{
    return pretypes;
}

const PreType &PreTypeManager::getPreType(const std::string &name)
{
    for (const PreType &p : pretypes)
    {
        if (p.getName() == name)
            return p;
    }

    return PreType("{null}", "{null}");
}
