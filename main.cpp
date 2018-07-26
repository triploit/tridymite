#include <iostream>

#include "runtime.hpp"
#include "cli/cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli;
    cli.parseArguments(std::vector<std::string>(argv + 1, argv + argc));
}