#include "cli.hpp"


void CLI::init(const std::vector<std::string> &names,
        const std::vector<std::string> &helps,
        const std::vector<int> &argc,
        const std::string &pname,
        const Version &v)
{
    CLI::arg_name = names;
    CLI::arg_help = helps;
    CLI::arg_argc = argc;

    CLI::program_name = pname;
    CLI::version = v;
}

bool CLI::parseArguments(std::vector<std::string> args) // parse the given arguments
{
    std::string type;

    for (std::string arg : args) // iterate through every argument
    {
        std::vector<std::string> family;

        if (arg[0] == '-')
        {
            family.clear();

            family = tstd::get_family(arg, arg_name);
            type = family[0];
            bool found = false;

            for (const std::string &an : arg_name)
            {
                std::string tmp;

                for (char c : an)
                {
                    if (c == ',')
                    {
                        if (tstd::add_prefix(tmp) == arg)
                            found = true;

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
                    if (tstd::add_prefix(tmp) == arg)
                        found = true;

                    for (std::string s : family)
                    {
                        if (CLI::arg_values.find(s) != CLI::arg_values.end())
                            arg_values.erase(CLI::arg_values.find(s));
                    }

                    tmp = "";
                }
            }

            if (!found)
            {
                printf(Translation::get("cli.argument.not_found").c_str(), arg.c_str());
                return false;
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
                std::cout << Translation::get("cli.argument.no_passed", false) << " (\"" << arg << "\")" << std::endl;
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
        int count = 0;

        for (char c : arg_name[i])
        {
            if (c == ',')
            {
                if (CLI::arg_values.find(tmp) != CLI::arg_values.end())
                {
                    count = 0;

                    for (const std::string &s : CLI::arg_values[tmp])
                    {
                        if (s != "<IGNORE_ME!>")
                            count++;
                    }

                    if (count != CLI::arg_argc[i])
                    {
                        if (CLI::arg_argc[i] != -1)
                        {
                            if (count > CLI::arg_argc[i])
                                printf(Translation::get("cli.argument.too_many").c_str(), tstd::add_prefix(tmp).c_str());
                            else
                                printf(Translation::get("cli.argument.too_few").c_str(), tstd::add_prefix(tmp).c_str());

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
                        if (count > CLI::arg_argc[i])
                            printf(Translation::get("cli.argument.too_many").c_str(), tstd::add_prefix(tmp).c_str());
                        else
                            printf(Translation::get("cli.argument.too_few").c_str(), tstd::add_prefix(tmp).c_str());

                        if (count != CLI::arg_argc[i])
                            return false;
                    }
                }
            }

            tmp = "";
        }
    }

    return true;
}

void CLI::printHelp(std::string arg0) // Generating the help page from the defined arguments.
{
    std::cout << program_name << " " << Translation::get("cli.version", false) << " " << version << std::endl << std::endl;
    int max_len = 0;
    int tmp = 0;

    for (const std::string &s : arg_name)
    {
        if (s.size() > max_len)
            max_len = (int) s.size();
    }

    for (const std::string &s : arg_help)
    {
        if (Translation::get("arguments.usage."+s, false).size() > tmp)
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

                gen_usage.push_back(Translation::get("arguments.usage."+arg_help[x], false));
                gen_name.push_back(tmp);
                tmp = "";

                found = true;
                continue;
            }

            tmp += c;
        }

        if (!tmp.empty())
        {

            if (x < Translation::get("arguments.help."+arg_help[x], false).size())
                gen_help.push_back(Translation::get("arguments.help."+arg_help[x], false));

            gen_usage.push_back(Translation::get("arguments.usage."+arg_help[x], false));
            gen_name.push_back(tmp);
            tmp = "";

            found = true;
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
