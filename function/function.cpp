#include "function.hpp"

#include <unistd.h>
#include <fstream>

#include <runtime.hpp>

Function::Function() {}

Function::Function(const std::string &_name, const std::string &_code) : name(_name), code(_code) {}

const std::string &Function::getCode()
{
    return code;
}

void Function::setCode(const std::string &code)
{
    this->code = code;
}

const std::string &Function::getName()
{
    return name;
}

void Function::setName(const std::string &name)
{
    this->name = name;
}

bool Function::operator!=(const Function &f)
{
    return (this->name != f.name || this->code != f.code);
}
