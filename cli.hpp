#ifndef CLI_HPP
#define CLI_HPP

#include <vector>

class CLI 
{
public:
    void parseArguments(std::vector<std::string> arguments);
    
    CLI() {}
};

#endif