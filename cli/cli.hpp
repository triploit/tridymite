#ifndef CLI_HPP
#define CLI_HPP

#include <vector>
#include "../package.hpp"

class CLI 
{
private:
    std::vector<std::string> arg_name;
    std::vector<std::string> arg_help;

    std::string program_name;
    Version version;

public:
    void init(const std::vector<std::string> &arg_name, const std::vector<std::string> &arg_help, std::string pname, Version v);
    bool parseArguments(std::vector<std::string> arguments);
    void printHelp();
    
    CLI() {}
};

#endif