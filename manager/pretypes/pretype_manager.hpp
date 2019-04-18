//
// Created by survari on 16.04.19.
//

#ifndef TRIDYMITE_PRETYPE_MANAGER_HPP
#define TRIDYMITE_PRETYPE_MANAGER_HPP

#include <script/script.hpp>
#include <pretype/pretype.hpp>

class PreTypeManager
{
private:
    static inline std::vector<PreType> pretypes;

public:
    static void load(std::string path);
    static bool existsPreType(const std::string &name);

    static const std::vector<PreType> &getPreTypes();
    static const PreType &getPreType(const std::string &name);
};


#endif //TRIDYMITE_PRETYPE_MANAGER_HPP
