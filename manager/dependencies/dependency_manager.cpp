//
// Created by survari on 12.01.19.
//

#include <runtime.hpp>
#include <std/tstd.hpp>
#include "dependency_manager.hpp"

void DependencyManager::checkDependencies(const Package &package_to_check)
{
    if (Runtime::no_dependencies)
        return;

    for (Package dependency : package_to_check.getDependencies())
    {
        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &x = Runtime::to_remove[i];

            if (x.getRepoName() == dependency.getRepoName() &&
                x.getGitUser() == dependency.getGitUser() &&
                x.getServer() == dependency.getServer())
            {
                printf(std::string(Translation::get("manager.dependencies.cant_remove_dependency", false)+" "+tstd::package_to_argument(package_to_check)+"\n").c_str(), tstd::package_to_argument(Runtime::to_remove[i]).c_str());
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
            }

            continue;
        }

        bool found = false;

        for (int x = 0; x < Runtime::to_install.size(); x++)
        {
            const Package &package_to_install = Runtime::to_install[x];

            if (package_to_install.getRepoName() == dependency.getRepoName() &&
                package_to_install.getGitUser() == dependency.getGitUser() &&
                package_to_install.getServer() == dependency.getServer())
            {
                found = true;
                
                Runtime::to_install.insert(Runtime::to_install.begin(), package_to_install);
                Runtime::to_install.erase(Runtime::to_install.begin()+x+1);
            }
        }

        if (!found)
        {
            for (const Package &installed_package : IPackagesManager::getInstalledPackages())
            {
                if (installed_package.getRepoName() == dependency.getRepoName() &&
                    installed_package.getGitUser() == dependency.getGitUser() &&
                    installed_package.getServer() == dependency.getServer())
                    found = true;
            }
        }

        if (!found)
        {
            std::cout << Translation::get("manager.dependencies.added_dependency", false) << " " << tstd::package_to_argument(dependency) << std::endl;
            DependencyManager::checkDependencies(dependency);
            Runtime::to_install.insert(Runtime::to_install.begin(), dependency);
        }
    }
}

std::vector<Package> DependencyManager::getPackagesConfig(std::vector<Package> packages)
{
    for (int i = 0; i < packages.size(); i++)
    {
        const Package &p = packages[i];
        const std::string file = Runtime::tmp_dir+"/tmp.yaml";

        std::string msg = Translation::get("general.loading_packages", false) + std::to_string(i+1) + "/" + std::to_string(packages.size());
        std::cout << msg << std::endl;

        if (!tstd::download_file(tstd::create_url(p, "raw/master/pkg/package.yaml"), file))
        {
            std::cout << std::endl;
            printf(Translation::get("general.package_not_found").c_str(), tstd::package_to_argument(p).c_str());
            Runtime::exit(1);
        }

        std::ofstream _of(file, std::ios::app);
        _of << std::endl << std::endl;
        _of << "gituser: " << p.getGitUser() << std::endl;
        _of << "reponame: " << p.getRepoName() << std::endl;
        _of << "server: " << p.getServer() << std::endl;
        _of.close();

        packages[i] = Package(YAML::LoadFile(file));
        std::remove(file.c_str());

        if ((i+1) < packages.size())
            printf("\033[1A");
    }

    return packages;
}
