#include "function.hpp"

#include <unistd.h>
#include <fstream>

#include <runtime.hpp>

Function::Function()
{
    name = "";
    code = "";
}

Function::Function(const std::string &name, const std::string &code)
{
    this->name = name;
    this->code = code;
}

void Function::run()
{
    std::string file_name = Runtime::tmp_dir+"/_tmp"+std::to_string(getpid())+"_fun_"+name+"_"+std::to_string(code.size())+".sh";

    std::fstream of(file_name, std::ios::out);
    of << "#!/usr/bin/bash" << std::endl << code << std::endl;

    std::system(std::string("bash "+file_name).c_str());

    Runtime::files_to_clean.push_back(file_name);
}

const std::string &Function::getCode()
{
    return code;
}

void Function::setCode(const std::string &code)
{
    this->code = code;
}

const std::string &Function::getName()
{
    return name;
}

void Function::setName(const std::string &name)
{
    this->name = name;
}

bool Function::operator!=(const Function &f)
{
    return (this->name != f.name || this->code != f.code);
}
