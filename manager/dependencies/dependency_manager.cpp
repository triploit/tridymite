//
// Created by survari on 12.01.19.
//

#include <runtime.hpp>
#include <std/tstd.hpp>
#include "dependency_manager.hpp"

void DependencyManager::checkDependencies(const Package &p)
{
    if (Runtime::no_dependencies)
        return;

    for (Package pkg : p.getDependencies())
    {
        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &x = Runtime::to_remove[i];

            if (x.getRepoName() == pkg.getRepoName() &&
                x.getGitUser() == pkg.getGitUser() &&
                x.getServer() == pkg.getServer())
            {
                std::cout << "info: can't remove " << tstd::package_to_argument(Runtime::to_remove[i]) << ": is a dependency of: " << tstd::package_to_argument(p) << std::endl;
                Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
            }

            continue;
        }

        bool found = false;

        for (int x = 0; x < Runtime::to_install.size(); x++)
        {
            const Package &i = Runtime::to_install[x];

            if (i.getRepoName() == pkg.getRepoName() &&
                i.getGitUser() == pkg.getGitUser() &&
                i.getServer() == pkg.getServer())
            {
                found = true;
                Runtime::to_install.insert(Runtime::to_install.begin(), i);
                Runtime::to_install.erase(Runtime::to_install.begin()+x+1);
            }
        }

        if (!found)
        {
            for (const Package &i : IPackagesManager::getInstalledPackages())
            {
                if (i.getRepoName() == pkg.getRepoName() &&
                    i.getGitUser() == pkg.getGitUser() &&
                    i.getServer() == pkg.getServer())
                    found = true;
            }
        }

        if (!found)
        {
            std::cout << "info: added dependency: " << tstd::package_to_argument(pkg) << std::endl;
            DependencyManager::checkDependencies(pkg);
            Runtime::to_install.insert(Runtime::to_install.begin(), pkg);
        }
    }
}

std::vector<Package> DependencyManager::getPackageConfig(std::vector<Package> packages)
{
    for (int i = 0; i < packages.size(); i++)
    {
        const Package &p = packages[i];
        const std::string file = Runtime::tmp_dir+"/tmp.yaml";

        std::string msg = "info: loading packages... " + std::to_string(i+1) + "/" + std::to_string(packages.size());
        std::cout << msg << std::endl;

        if (!tstd::download_file(tstd::create_url(p, "raw/master/pkg/package.yaml"), file))
        {
            std::cout << std::endl;
            std::cout << "error: package " << tstd::package_to_argument(p) << " not found!" << std::endl;
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
