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
        std::cout << Translation::get("script.parse_file.file_not_found");
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

        file_content = code;

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
                    printf(Translation::get("script.parse_file.too_many_brackets").c_str(), line_count, file_name.c_str());
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
                    printf(Translation::get("script.parse_file.unfinished_declaration").c_str(), tokens[i].getLine());
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
                    printf(Translation::get("script.parse_file.unfinished_declaration").c_str(), tokens[i].getLine());
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

void Script::runFunction(std::string f, const std::string &token)
{
    if (existsFunction(f))
    {
        std::string file_name = Runtime::tmp_dir+"/_tmp"+token+"_"+std::to_string(getFunctions().size())+".sh";
        std::fstream of(file_name, std::ios::out);

        if (file_content.find("sudo") != std::string::npos || file_content.find("su root") != std::string::npos)
        {
            std::cout << "error: unsave script: script uses sudo command!" << std::endl;
            Runtime::exit(1);
        }

        of << "#!/usr/bin/bash" << "\n\n" << file_content << std::endl;

        std::system(std::string("source "+file_name+"; "+f).c_str());
        Runtime::files_to_clean.push_back(file_name);
    }
    else
    {
        std::cout << "error: can't run function: " << f << ": function not found!" << std::endl;
        Runtime::exit(1);
    }
}

const std::string &Script::getContent()
{
    return file_content;
}
