#include "variable.hpp"

Variable::Variable(const std::string &name, const std::string &value)
{
    this->name = name;
    this->value = value;
}

const std::string &Variable::getName() const
{
    return name;
}

const std::string &Variable::getValue() const
{
    return value;
}

void Variable::setName(const std::string &name)
{
    this->name = name;
}

void Variable::setValue(const std::string &value)
{
    this->value = value;
}
