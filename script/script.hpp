/*

THE WHOLE PACKAGE.SH CONTENT GOES HERE, WITH ALL FUNCTIONS

*/

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <vector>
#include <function.hpp>
#include <package/package.hpp>

class Script
{
private:
    std::vector<Function> functions;
    std::string file_name;
    std::string file_content;

    Function null_function;

    bool isdelim(char c);
    void parse_file(const std::string &f);

public:
    Script();
    Script(const std::string &file_name);

    const std::vector<Function> &go();
    const std::vector<Function> &getFunctions();

    const std::string &getContent();
    const std::string &getFileName();

    Function &getFunction(const std::string &name);
    bool existsFunction(const std::string &name);

    void addFunction(const Function &f);
    void runFunction(const std::string &function, const std::string &token);
};

#endif