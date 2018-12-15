#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <vector>

#include "package.hpp"

class Runtime
{
public:
    std::vector<Package> to_install;
    std::vector<Package> to_remove;
    std::vector<Package> to_update;

} Runtime;

#endif