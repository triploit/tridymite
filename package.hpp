#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <string>
#include <vector>

#include "version.hpp"

class Package
{
public:
    std::string gituser;
    std::string gitserver;
    std::string description;

    std::vector<std::string> contributors;
    std::vector<Package> dependencies;

    Version version;

    Package()
    {
        version = Version("0.0.0.0");
    }
};

#endif