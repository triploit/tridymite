#include <vector>
#include <string>
#include <iostream>

#include "cli.hpp"

void CLI::installPackage(Package p)
{

}

void CLI::removePackage(Package p)
{

}

void CLI::updatePackage(Package p)
{

}

void CLI::parseArguments(std::vector<std::string> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        std::cout << args[i] << std::endl;
    }
}