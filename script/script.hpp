/*

THE WHOLE PACKAGE.SH CONTENT GOES HERE, WITH ALL FUNCTIONS

*/

#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include <vector>
#include <function.hpp>

class Script
{
private:
    std::vector<Function> functions;
    std::string file_name;

    Function null_function;

    bool isdelim(char c);
    void parse_file(std::string f);

public:
    Script();
    Script(const std::string &file_name);

    const std::vector<Function> &getFunctions();
    Function &getFunction(const std::string &name);

    bool existsFunction(const std::string &name);
    void addFunction(Function f);

    const std::string &getFileName();

    const std::vector<Function> &go();
};

#endif