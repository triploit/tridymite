//
// Created by survari on 01.01.19.
//

#include <iostream>
#include <yaml-cpp/yaml.h>

#include <runtime.hpp>

std::string Translation::get(const std::string &s, const bool &nl, const bool &bnl)
{
    if (sentences[s])
    {
        std::string str = std::string(sentences[s].as<std::string>());

        if (nl)
            str = str + "\n";
        if (bnl)
            str = "\n" + str;

        return str;
    }
    else
    {
        if (!fallback[s])
        {
            std::cout << "error: fatal error in tridymite: sentences \"" << s << "\" doesn't exist in fallback!" << std::endl;
            Runtime::exit(1, false);
        }
        else
        {
            std::string str = std::string(fallback[s].as<std::string>());

            if (nl)
                str = str + "\n";
            if (bnl)
                str = "\n" + str;

            return ("[?] "+str);
        }
    }

    return "<NOT FOUND>";
}

void Translation::loadConfig(std::string path)
{
    fallback = YAML::Load(R"V0G0N(
# cli

cli.argument.not_found: "error: argument \"%s\" not found!"
cli.argument.no_passed: "error: no argument passed!"
cli.argument.too_many: "error: too many parameters for argument \"%s\""
cli.argument.too_few: "error: too few parameters for argument \"%s\""
cli.version: "version"

# manager/dependencies

# manager/install

# manager/packages

# manager/remove

# script

script.parse_file.file_not_found: "error: can't go on without a file."
script.parse_file.to_many_brackets: "error: line %d: to many \"}\" in file %s!"
script.parse_file.unfinished_declaration: "error: line %d: unfinished declaration of function in build script."

# std

arguments.help.help: "shows this help page"
arguments.help.install: "install a package"
arguments.help.remove: "remove a package"
arguments.help.update: "update a package"
arguments.help.update_all: "update all installed packages"
arguments.help.path: "installing a package from the source in the given directory"
arguments.help.verbose: "show extended (verbose) output"
arguments.help.insecure: "installing packages without asking security questions"
arguments.help.server: "setting the server for all package arguments"
arguments.help.user: "setting the user for all package arguments"
arguments.help.version: "show the version of tridymite"
arguments.help.language_file: "loading all messages from the given language file"
arguments.help.list_packages: "list all installed packages"
arguments.help.packages: "get information of package"
arguments.help.description: "shows description of a package"
arguments.help.already_installed: "don't skip already installed packages"
arguments.help.testing: "testing stuff"
arguments.help.no_deps: "don't check dependencies"
arguments.help.force: "don't look for file conflicts"
arguments.help.local: "if possible, try to install a package locally"

arguments.usage.help: ""
arguments.usage.install: "<package> ..."
arguments.usage.remove: "<package> ..."
arguments.usage.update: "<package> ..."
arguments.usage.update_all: ""
arguments.usage.path: "<path>"
arguments.usage.verbose: ""
arguments.usage.insecure: ""
arguments.usage.server: "<server>"
arguments.usage.user: "<username>"
arguments.usage.version: ""
arguments.usage.language_file: "<file>"
arguments.usage.list_packages: ""
arguments.usage.packages: "<package>"
arguments.usage.description: "<package>"
arguments.usage.already_installed: ""
arguments.usage.no_deps: ""
arguments.usage.force: ""
arguments.usage.testing: "???"
arguments.usage.local: ""

arguments.package: "<package> = <git-user>:<git-repo>[@<git-server>] (git-server is github.com in default)"

# runtime

runtime.not_installed: "error: it seems tridymite isn't installed yet.\nIf you have the source code here, try running the install.sh script as root."
runtime.tmp_permission_error: "error: couldn't create tmp-directory!: "
runtime.clean_files.error: "error: couldn't delete file: %s"
runtime.clean_directories.error: "error: couldn't delete directory: %s"

runtime.clear_up_all_tmps: "error: error at cleaning up temporary files or directories."
runtime.exit: "exiting with code %d"
)V0G0N");

    if (std::ifstream(path).is_open())
    {
        sentences = YAML::LoadFile(path);
    }
    else
    {
        std::cout << "warning: language file " << path << " not found!" << std::endl;
        std::cout << "Using fallback..." << std::endl;
    }
}

