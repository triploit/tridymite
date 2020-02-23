#include <fstream>
#include <runtime.hpp>
#include <std/tstd.hpp>
#include "pretype.hpp"

PreType::PreType(const std::string &file, const std::string &name)
{
    this->name = name;
    this->full_file_name = file;

    std::ifstream fi(file);

    if (!fi.is_open())
    {
        std::cout << "error: pretype-file \"" << file << "\" not found!";
        Runtime::exit(1);
    }
    else
    {
        std::string line = "";

        while(std::getline(fi, line))
        {
            if(line.size() > 2)
            {
                if (line[0] == '#' && line[1] == '%')
                {
                    line = tstd::trim(line.substr(2, line.size()));
                    var_names = tstd::split(line, ',');
                }
            }
        }
    }
}

void PreType::runScript(const std::vector<Variable> &variables, const std::string &token)
{
    if (variables.size() < var_names.size())
    {
        std::cout << "error: pretypes: pretype " << name << ": some variables were'nt set:";

        for (int i = (variables.size()-1); i < var_names.size(); i++)
            std::cout << "    - " << var_names[i] << std::endl;
    }
    else
    {
        std::vector<std::string> missing;

        for (const std::string &s : var_names)
        {
            bool found = false;

            for (const Variable &v : variables)
            {
                if (v.getName() == s)
                    found = true;
            }

            if (!found)
                missing.push_back(s);
        }

        if (missing.size() != 0)
        {
            std::cout << "error: pretypes: pretype " << name << ": some variables were'nt set:";

            for (const std::string &s : missing)
                std::cout << "    - " << s << std::endl;

            Runtime::exit(1);
        }
    }

    Script::pureRunWithVariables(*this, variables, token);
}

const std::string& PreType::getName() const
{
    return name;
}

const std::vector<std::string> &PreType::getNeededVariables()
{
    return var_names;
}

const std::string &PreType::getFullFileName() const
{
    return full_file_name;
}

PreType::PreType()
{
    this->name = "none";
    this->full_file_name = "none";
}

void PreType::setName(const std::string &name)
{
    this->name = name;
}

void PreType::setFullFileName(const std::string &file)
{
    this->full_file_name = name;
}
