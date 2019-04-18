#include <function.hpp>
#include <fstream>
#include <runtime.hpp>
#include <std/tstd.hpp>
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

void Script::parse_file(const std::string &f)
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

        bool delim = false;

        bool brackets = false;
        int bracket_count = 0;

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

void Script::addFunction(const Function &f)
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

void Script::runFunction(const std::string &function, const std::string &token)
{
    if (existsFunction(function))
    {
        std::string file_name = Runtime::tmp_dir+"/_tmp"+token+"_"+std::to_string(getFunctions().size())+".sh";
        std::fstream of(file_name, std::ios::out);

        if (file_content.find("sudo") != std::string::npos || file_content.find("su root") != std::string::npos)
        {
            std::cout << Translation::get("script.unsave_script", false) << std::endl;
            Runtime::exit(1);
        }

        of << "#!/usr/bin/bash" << "\n\n" << file_content << std::endl;

        std::system(std::string("source "+file_name+"; "+function).c_str());
        Runtime::files_to_clean.push_back(file_name);
    }
    else
    {
        printf(Translation::get("script.function_not_found").c_str(), function.c_str());
        Runtime::exit(1);
    }
}

const std::string &Script::getContent()
{
    return file_content;
}

void Script::pureRunWithVariables(const PreType &p, const std::vector<Variable> &variables, const std::string &token)
{
    std::ifstream t(p.getFullFileName());
    std::string content((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());

    std::string file_name = Runtime::tmp_dir+"/_GENtmp"+std::to_string(variables.size())+"_"+token+"_"+p.getName()+".sh";
    std::fstream of(file_name, std::ios::out);

    if (content.find("sudo") != std::string::npos || content.find("su root") != std::string::npos)
    {
        std::cout << Translation::get("script.unsave_script", false) << std::endl;
        Runtime::exit(1);
    }

    for (const Variable &v : variables)
    {
        content = tstd::replace(content, std::string("${"+v.getName()+"}"), v.getValue());
    }

    of << "#!/usr/bin/bash" << "\n\n" << content << std::endl;

    std::system(std::string("bash "+file_name).c_str());
    Runtime::files_to_clean.push_back(file_name);
}
