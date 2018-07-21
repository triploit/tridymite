#include <vector>
#include <string>
#include <iostream>

#include "cli.hpp"

void CLI::parseArguments(std::vector<std::string> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        std::cout << args[i] << std::endl;
    }
}