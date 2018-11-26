#include <vector>
#include <string>
#include <iostream>

#include <std/tstd.hpp>

#include <cli/cli.hpp>
#include "cli.hpp"


void CLI::init(const std::vector<std::string> &names,
        const std::vector<std::string> &helps,
        const std::vector<int> &argc,
        const std::vector<std::string> &usage,
        const std::string &pname,
        const Version &v)
{
    CLI::arg_name = names;
    CLI::arg_help = helps;
    CLI::arg_argc = argc;
    CLI::arg_usage = usage;

    CLI::program_name = pname;
    CLI::version = v;
}

bool CLI::parseArguments(std::vector<std::string> args)
{
    std::string type;

    for (std::string arg : args)
    {
        std::vector<std::string> family;

        if (arg[0] == '-')
        {
            family.clear();

            family = tstd::get_family(arg, arg_name);
            type = family[0];

            for (const std::string &an : arg_name)
            {
                std::string tmp;

                for (char c : an)
                {
                    if (c == ',')
                    {
                        for (const std::string &s : family)
                        {
                            if (CLI::arg_values.find(s) != CLI::arg_values.end())
                                arg_values.erase(CLI::arg_values.find(s));
                        }

                        tmp = "";
                        continue;
                    }

                    tmp += c;
                }

                if (!tmp.empty())
                {
                    for (std::string s : family)
                    {
                        if (CLI::arg_values.find(s) != CLI::arg_values.end())
                            arg_values.erase(CLI::arg_values.find(s));
                    }

                    tmp = "";
                }
            }

            std::string key = type;

            if (!family.empty())
                key = family[0];

            CLI::arg_values[key].push_back("<IGNORE_ME!>");
        }
        else
        {
            if (type.empty())
            {
                std::cout << "error: no argument passed! (\"" << arg << "\")" << std::endl;
                return false;
            }
            else
            {
                std::string key = type;

                if (!family.empty())
                    key = family[0];

                CLI::arg_values[key].push_back(arg);
            }
        }
    }

    for (int i = 0; i < arg_name.size(); i++)
    {
        std::string tmp;

        for (char c : arg_name[i])
        {
            if (c == ',')
            {
                if (CLI::arg_values.find(tmp) != CLI::arg_values.end())
                {
                    int count = 0;

                    for (const std::string &s : CLI::arg_values[tmp])
                    {
                        if (s != "<IGNORE_ME!>")
                            count++;
                    }

                    if (count != CLI::arg_argc[i])
                    {
                        if (CLI::arg_argc[i] != -1)
                        {
                            std::cout << "error: too much parameters for argument \"" << tstd::add_prefix(tmp) << "\"" << std::endl;
                            return false;
                        }
                    }
                }

                tmp = "";
                continue;
            }

            tmp += c;
        }

        if (!tmp.empty())
        {
            if (CLI::arg_values.find(tmp) != CLI::arg_values.end())
            {
                if (CLI::arg_values[tmp].size() != CLI::arg_argc[i])
                {
                    if (CLI::arg_argc[i] != -1)
                    {
                        std::cout << "error: too much parameters for argument \"" << tstd::add_prefix(tmp) << "\"" << std::endl;
                        return false;
                    }
                }
            }

            tmp = "";
        }
    }

    return true;
}

void CLI::printHelp(std::string arg0)
{
    std::cout << program_name << " version " << version << std::endl << std::endl;
    int max_len = 0;
    int tmp = 0;

    for (const std::string &s : arg_name)
    {
        if (s.size() > max_len)
            max_len = (int) s.size();
    }

    for (const std::string &s : arg_usage)
    {
        if (s.size() > tmp)
            tmp = (int) s.size();
    }

    max_len = max_len + tmp;

    std::vector<std::string> gen_help;
    std::vector<std::string> gen_name;
    std::vector<std::string> gen_usage;

    for (int x = 0; x < arg_name.size(); x++)
    {
        std::string tmp;
        bool found = false;

        for (const char &c : arg_name[x])
        {
            if (c == ',')
            {
                gen_help.push_back("<NOTHING!>");

                gen_usage.push_back(arg_usage[x]);
                gen_name.push_back(tmp);
                tmp = "";

                found = true;
                continue;
            }

            tmp += c;
        }

        if (!tmp.empty())
        {

            if (x < arg_help.size())
                gen_help.push_back(arg_help[x]);

            gen_usage.push_back(arg_usage[x]);
            gen_name.push_back(tmp);
            tmp = "";
        }

        if (found)
        {
            found = false;
            gen_name.push_back("<NEWLINE!>");
        }
    }

    int count = 0;
    for (int i = 0; i < gen_name.size(); i++)
    {
        if (gen_name[i] == "<NEWLINE!>")
        {
            std::cout << std::endl;
            continue;
        }

        std::string space;
        std::string prefix;

        if (gen_name[i].size() > 2)
            prefix = "--";
        else
            prefix = "-";

        int gen_count = (max_len - gen_name[i].size() - prefix.size() - gen_usage[count].size());

        for (int x = 0; x < gen_count; x++)
        {
            if (gen_count < 0)
                break;

            space += " ";
        }

        if (count < gen_help.size() && gen_help[count] != "<NOTHING!>")
        {
            std::vector<std::string> split_help = tstd::split(gen_help[count], ' ');
            std::string help;

            if (gen_help[count].size() > 30)
            {
                std::string help_space;
                std::string help_part;

                int size = (int) std::string(prefix + gen_name[i] + gen_usage[count] + space).size()+8;
                int line = 0;

                for (int x = 0; x < size; x++)
                {
                    help_space += " ";
                }

                for (const std::string &help_pp : split_help)
                {
                    if (help_part.size() >= 20)
                    {
                        if (line > 0)
                            help += help_part + "\n" + help_space + help_pp + " ";
                        else
                            help += help_part + "\n" + help_space + help_pp + " ";

                        line++;
                        help_part = "" ;
                    }
                    else
                    {
                        help_part += help_pp + " ";
                    }

                }

                help += help_part;
            }
            else
                help = gen_help[count];

            std::cout << "    " << prefix << gen_name[i] << " " << gen_usage[count] << space << " - " << help << std::endl;
        }
        else
        {
            std::string postfix;

            if (gen_help[count] == "<NOTHING!>")
                postfix = ",";

            std::cout << "    " << prefix << gen_name[i] << postfix << std::endl;
        }

        count += 1;
    }
}

bool CLI::argumentGiven(std::string name)
{
    if (CLI::arg_values.find(name) != CLI::arg_values.end())
        return true;

    return false;
}

std::vector<std::string> CLI::getParameters(std::string argument)
{
    std::vector<std::string> result;

    for (const std::string &s : CLI::arg_values[argument])
    {
        if (s != "<IGNORE_ME!>")
            result.push_back(s);
    }

    return result;
}
