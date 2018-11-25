#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <string>
#include <vector>

#include "version.hpp"
#include "script.hpp"

class Package
{
public:
    std::string reponame;
    std::string gituser;
    std::string name;
    std::string description;
    std::string information;
    std::string server;
    Script functions;

    std::vector<Package> dependencies;
    std::vector<std::string> authors;

    Version version;

    Package();
};

#endif