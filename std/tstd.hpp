#ifndef TRIDYMITE_TSTD_HPP
#define TRIDYMITE_TSTD_HPP

#include <iostream>
#include <vector>
#include <package/package.hpp>
#include <runtime.hpp>

namespace tstd
{
    Package parse_package(const std::string &package);

    std::string add_prefix(std::string arg); // Add a prefix (- or --) to argument (like "help" -> "--help"; "i" -> "-i")
    std::string create_url(Package p, std::string postfix="");

    std::vector<Package> parse_package_arguments(const std::vector<std::string> &packages);
    std::vector<std::string> split(std::string s, char delim); // Split a string by a delimiter
    std::vector<std::string> get_family(std::string arg, std::vector<std::string> arg_name); // Getting the family of an argument, e.g. "i" is contained in the family of "i,install"

    void dont_rush(std::string s);
};


#endif //TRIDYMITE_TSTD_HPP
