#include <vector>
#include <string>
#include <iostream>

#include <std/tstd.hpp>

#include <cli/cli.hpp>

void CLI::init(const std::vector<std::string> &names, const std::vector<std::string> &helps, std::string pname, Version v)
{
    CLI::arg_name = names;
    CLI::arg_help = helps;

    CLI::program_name = pname;
    CLI::version = v;
}

bool CLI::parseArguments(std::vector<std::string> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        std::cout << args[i] << std::endl;
    }

    return true;
}

void CLI::printHelp()
{
    std::cout << program_name << " version " << version << std::endl << std::endl;
    int max_len = 0;

    for (std::string s : arg_name)
    {
        if (s.size() > max_len)
            max_len = (int) s.size();
    }

    std::vector<std::string> gen_help;
    std::vector<std::string> gen_name;

    for (int x = 0; x < arg_name.size(); x++)
    {
        std::string tmp;
        bool found = false;

        for (int i = 0; i < arg_name[x].size(); i++)
        {
            if (arg_name[x][i] == ',')
            {
                gen_help.push_back("<NOTHING!>");
                gen_name.push_back(tmp);
                tmp = "";

                found = true;
                continue;
            }

            tmp += arg_name[x][i];
        }

        if (!tmp.empty())
        {

            if (x < arg_help.size())
                gen_help.push_back(arg_help[x]);

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

        for (int x = 0; x < (max_len-gen_name[i].size()-prefix.size()); x++)
        {
            if ((max_len-gen_name[i].size()-prefix.size()) < 0)
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

                int size = (int) std::string(prefix + gen_name[i] + space).size()+7;
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

            std::cout << "    " << prefix << gen_name[i] << space << " - " << help << std::endl;
        }
        else
        {
            std::string postfix = "";

            if (gen_help[count] == "<NOTHING!>")
                postfix = ",";

            std::cout << "    " << prefix << gen_name[i] << postfix << std::endl;
        }

        count += 1;
    }
}
