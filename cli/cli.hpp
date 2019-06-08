#ifndef CLI_HPP
#define CLI_HPP

#include <vector>
#include <map>
#include <package.hpp>
#include <string>
#include <iostream>

#include <std/tstd.hpp>

#ifndef ARGUMENT_COUNT
#define ARGUMENT_COUNT 12
#endif

class CLI 
{
private:
    std::vector<std::string> arg_name;
    std::vector<std::string> arg_help;
    std::vector<int> arg_argc;
    std::vector<void (*)(const std::vector<std::string>&, CLI* cli)> arg_fun;

    std::map<std::string, std::vector<std::string>> arg_values;

    std::string program_name;
    semver::Version version;


public:
    void init(const std::vector<std::string> &args,
            const std::vector<std::string> &helps,
            const std::vector<int> &argc,
            const std::string &pname,
            const semver::Version &v,
            const std::vector<void (*)(const std::vector<std::string>&, CLI* cli)> &fun);

    void printHelp(const std::string &arg0);
    void runArguments();

    bool parseArguments(const std::vector<std::string> &arguments);
    bool argumentGiven(const std::string &name);

    std::vector<std::string> getParameters(const std::string &argument);
    
    CLI() {}
};

#endif