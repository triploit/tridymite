//
// Created by survari on 02.06.19.
//

#ifndef TRIDYMITE_KEEP_TEMP_HPP
#define TRIDYMITE_KEEP_TEMP_HPP

#include <vector>
#include <string>

#include <runtime.hpp>
#include <cli/cli.hpp>

void __argument_keep_tmp(const std::vector<std::string> &parameters, CLI* cli)
{
    Runtime::keep_tmp = true;
}

#endif //TRIDYMITE_KEEP_TEMP_HPP
