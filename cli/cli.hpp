#ifndef CLI_HPP
#define CLI_HPP

#include <vector>
#include "../package.hpp"

class CLI 
{
public:
    void installPackage(Package package);
    void removePackage(Package package);
    void updatePackage(Package package);
    void parseArguments(std::vector<std::string> arguments);
    
    CLI() {}
};

#endif