//
// Created by survari on 16.12.18.
//

#include "token.hpp"

Token::Token(const std::string &value, const int &line)
{
    this->value = value;
    this->line = line;
}

const int& Token::getLine()
{
    return line;
}

void Token::setLine(const int &line)
{
    this->line = line;
}

const std::string& Token::getValue()
{
    return value;
}

void Token::setValue(const std::string &value)
{
    this->value = value;
}
