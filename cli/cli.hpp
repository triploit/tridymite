#ifndef CLI_HPP
#define CLI_HPP

#include <vector>
#include <map>
#include "../package.hpp"

class CLI 
{
private:
    std::vector<std::string> arg_name;
    std::vector<std::string> arg_help;
    std::vector<std::string> arg_usage;
    std::vector<int> arg_argc;

    std::map<std::string, std::vector<std::string>> arg_values;

    std::string program_name;
    Version version;

public:
    void init(const std::vector<std::string> &args,
            const std::vector<std::string> &helps,
            const std::vector<int> &argc,
            const std::vector<std::string> &usage,
            const std::string &pname,
            const Version &v);

    void printHelp(std::string arg0);

    bool parseArguments(std::vector<std::string> arguments);
    bool argumentGiven(std::string name);

    std::vector<std::string> getParameters(std::string argument);
    
    CLI() {}
};

#endif