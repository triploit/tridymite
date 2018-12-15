#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>

class Function 
{
private:
    std::string name;
    std::string code;

public:
    Function();
    Function(const std::string &name, const std::string &code);

    const std::string &getName();
    void setName(const std::string &name);

    const std::string &getCode();
    void setCode(const std::string &code);
};

#endif