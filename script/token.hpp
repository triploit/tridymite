//
// Created by survari on 16.12.18.
//

#ifndef TRIDYMITE_TOKEN_HPP
#define TRIDYMITE_TOKEN_HPP

#include <string>

class Token
{
private:
    std::string value;
    int line;

public:
    Token(const std::string &value, const int &line);

    const std::string &getValue();
    void setValue(const std::string &value);

    const int &getLine();
    void setLine(const int &line);
};

#endif //TRIDYMITE_TOKEN_HPP
