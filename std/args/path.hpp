#ifndef TRIDYMITE_PATH_HPP
#define TRIDYMITE_PATH_HPP

#include <vector>
#include <manager/install/installation_manager.hpp>

void __argument_path(const std::vector<std::string> &parameters, CLI* cli)
{
    std::cout << Translation::get("main.installing_local_path", false) << " " << cli->getParameters("p")[0] << std::endl;
    chdir(cli->getParameters("p")[0].c_str());

    if (!InstallationManager::localPackage(cli->getParameters("p")[0]))
    {
        Package package = Package(YAML::LoadFile(cli->getParameters("p")[0]+"/pkg/package.yaml"));

        if (package.isDependency())
        {
            printf(Translation::get("manager.install.cant_continue_installation_is_dep").c_str(),
                   package.getDependantPackage().c_str());
            Runtime::exit(1);
        }
    }
}

#endif //TRIDYMITE_HELP_HPP
