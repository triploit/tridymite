#include <function.hpp>
#include <fstream>
#include <runtime.hpp>
#include "script.hpp"
#include "token.hpp"

Script::Script()
{
    null_function = Function("{null}", "{null}");
}

Script::Script(const std::string &file_name)
{
    null_function = Function("{null}", "{null}");
    this->file_name = file_name;
}

bool Script::isdelim(char c)
{
    std::vector<char> delims = {
            ' ',
            '\n',
            '\t'
    };

    for (const char &d : delims)
    {
        if (d == c)
            return true;
    }

    return false;
}

void Script::parse_file(std::string f)
{
    std::vector<Function> fs;

    if (file_name.empty())
    {
        std::cout << "error: can't go on without a file." << std::endl;
        Runtime::exit(1);
    }
    else
    {
        std::fstream istream(file_name, std::ios::in);

        std::string code;
        std::string line;

        std::vector<Token> tokens;

        while (std::getline(istream, line))
        {
            code += line + "\n";
        }

        bool brackets = false;
        int bracket_count = 0;

        bool delim = false;
        bool str = false;

        std::string tmp;
        int line_count = 1;

        for (const char &c : code)
        {
            if (c == '\n')
                line_count++;

            if (c == '{')
            {
                if (bracket_count >= 1)
                    tmp += c;

                bracket_count++;
                brackets = true;

                continue;
            }

            if (c == '}')
            {
                bracket_count--;

                if (bracket_count == 0)
                {
                    tokens.push_back(Token(tmp, line_count));
                    tmp = "";

                    brackets = false;
                }
                else if (bracket_count < 0)
                {
                    std::cout << "error: line " << line_count << ": too many \"}\" in file \"" << file_name << "\"!" << std::endl;
                    Runtime::exit(1);
                }
                else
                    tmp += c;

                continue;
            }

            if (!brackets)
            {
                if (isdelim(c))
                {
                    delim = true;
                    tokens.push_back(Token(tmp, line_count));
                    tmp = "";
                }
                else
                {
                    tmp += c;
                }
            }
            else
                tmp += c;
        }

        for (int i = 0; i < tokens.size(); i++)
        {
            if (tokens[i].getValue() == "function" ||
                tokens[i].getValue() == "func")
            {
                if ((i+2) >= tokens.size())
                {
                    std::cout << "error: line " << tokens[i].getLine() << ": unfinished decleration in build script." << std::endl;
                    Runtime::exit(1);
                }

                fs.push_back(Function(tokens[i+1].getValue(), tokens[i+2].getValue()));

                i += 2;
                continue;
            }
            else if (tokens[i].getValue() == "()")
            {
                if ((i+1) >= tokens.size() || (i-1) < 0)
                {
                    std::cout << "error: line " << tokens[i].getLine() << ": unfinished decleration in build script." << std::endl;
                    Runtime::exit(1);
                }

                fs.push_back(Function(tokens[i-1].getValue(), tokens[i+1].getValue()));
            }
        }
    }

    functions = fs;
}

const std::vector<Function>& Script::getFunctions()
{
    return functions;
}

const std::vector<Function> &Script::go()
{
    parse_file(file_name);
    return functions;
}

Function &Script::getFunction(const std::string &name)
{
    for (Function &func : functions)
    {
        if (func.getName() == name)
            return func;
    }

    return null_function;
}

void Script::addFunction(Function f)
{
    functions.push_back(f);
}

bool Script::existsFunction(const std::string &name)
{
    return (null_function != getFunction(name));
}

const std::string& Script::getFileName()
{
    return file_name;
}