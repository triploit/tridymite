#ifndef TRIDYMITE_VARIABLE_HPP
#define TRIDYMITE_VARIABLE_HPP


#include <string>

class Variable
{
private:
    std::string name;
    std::string value;

public:
    Variable(const std::string &name, const std::string &value);

    const std::string &getName() const;
    const std::string &getValue() const;

    void setName(const std::string &name);
    void setValue(const std::string &value);
};


#endif //TRIDYMITE_VARIABLE_HPP
