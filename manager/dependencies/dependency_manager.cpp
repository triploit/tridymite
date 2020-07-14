#include <runtime.hpp>
#include <std/tstd.hpp>
#include <thread>
#include "dependency_manager.hpp"

void DependencyManager::checkDependencies(const Package package_to_check)
{
    if (Runtime::no_dependencies)
        return;

    for (Package dependency : package_to_check.getDependencies())
    {
        dependency = getPackagesConfig({ dependency }, false)[0];
        dependency.setAddedBy(tstd::package_to_argument(package_to_check));

        for (int i = 0; i < Runtime::to_remove.size(); i++)
        {
            const Package &x = Runtime::to_remove[i];

            if (x.getRepoName() == dependency.getRepoName() &&
                x.getGitUser() == dependency.getGitUser() &&
                x.getServer() == dependency.getServer())
            {
                bool found = false;

                for (const Package &search : Runtime::to_remove)
                {
                    if (search.getRepoName() == package_to_check.getRepoName() &&
                        search.getGitUser() == package_to_check.getGitUser() &&
                        search.getServer() == package_to_check.getServer())
                        found = true;
                }

                if (!found)
                {
                    printf(std::string(Translation::get("manager.dependencies.cant_remove_dependency", false)+" "+tstd::package_to_argument(package_to_check)+"\n").c_str(), tstd::package_to_argument(Runtime::to_remove[i]).c_str());
                    Runtime::to_remove.erase(Runtime::to_remove.begin()+i);
                }
            }

            continue;
        }

        bool found = false;

        for (const Package &installed_package : IPackagesManager::getInstalledPackages())
        {
            if (installed_package.getRepoName() == dependency.getRepoName() &&
                installed_package.getGitUser() == dependency.getGitUser())
            {
                if (installed_package.getVersion() != dependency.getVersion() && installed_package.isDependency())
                {
                    std::cout << std::endl;
                    printf(Translation::get("manager.dependencies.dependency_version_conflict").c_str(),
                           dependency.getAddedBy().c_str(),
                           installed_package.getDependantPackage().c_str(),

                           std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str(),

                           dependency.getAddedBy().c_str(),
                           dependency.getVersion().ToString().c_str(),
                           std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str(),

                           installed_package.getDependantPackage().c_str(),
                           installed_package.getVersion().ToString().c_str(),
                           std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str());

                    if (!IPackagesManager::isPackageInstalledNV(tstd::parse_package(dependency.getAddedBy())))
                        std::cout << std::endl << Translation::get("manager.install.aborting_installation");

                    Runtime::exit(1);
                }
                else if (installed_package.getVersion() == dependency.getVersion())
                    found = true;
            }
        }

        if (!found)
        {
            for (int x = 0; x < Runtime::to_install.size(); x++)
            {
                const Package &package_to_install = Runtime::to_install[x];

                if (package_to_install.getRepoName() == dependency.getRepoName() &&
                    package_to_install.getGitUser() == dependency.getGitUser())
                {
                    if (package_to_install.getVersion() != dependency.getVersion())
                    {
                        std::cout << std::endl;
                        printf(Translation::get("manager.dependencies.dependency_version_conflict").c_str(),
                            dependency.getAddedBy().c_str(),
                            package_to_install.getAddedBy().c_str(),

                            std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str(),

                            dependency.getAddedBy().c_str(),
                            dependency.getVersion().ToString().c_str(),
                            std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str(),

                            package_to_install.getAddedBy().c_str(),
                            package_to_install.getVersion().ToString().c_str(),
                            std::string(dependency.getGitUser()+":"+dependency.getRepoName()).c_str());

                        if (!IPackagesManager::isPackageInstalledNV(tstd::parse_package(dependency.getAddedBy())))
                            std::cout << std::endl << Translation::get("manager.install.aborting_installation");

                        Runtime::exit(1);
                    }

                    found = true;

                    Runtime::to_install.insert(Runtime::to_install.begin(), package_to_install);
                    Runtime::to_install.erase(Runtime::to_install.begin()+x+1);
                }
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

std::vector<Package> DependencyManager::getPackagesConfig(std::vector<Package> packages, bool show_message)
{
    std::vector<std::thread> threads;
    int count;

    for (int i = 0; i < packages.size(); i++)
    {
        count += 1;

        if (count > 20) {
            for (int x = 0; x < threads.size(); x++)
            {
                if (threads[x].joinable())
                    threads[x].join();
            }

            count = 0;
            threads.clear();
        }

        threads.push_back(std::thread([](int i, const std::vector<Package> &packages)
        {
            const Package &p = packages[i];
            const std::string file =
            Runtime::tmp_dir + "/_DMIT_tmp" + std::to_string(i) + ".yaml";

            if (!tstd::download_file(tstd::create_url(p, "raw/" + p.getBranch() + "/pkg/package.yaml"), file))
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
            _of << "branch: " << p.getBranch() << std::endl;
            _of.close();
        }, i, packages));
    }

    count = 0;

    for (std::thread &t : threads)
    {
        if (t.joinable())
        {
            count += 1;

            if (show_message)
                std::cout << Translation::get("general.loading_packages", false) + " " +
                             std::to_string(count) + "/" +
                             std::to_string(packages.size()) << std::endl;

            std::cin.sync();
            t.join();

            if (count < packages.size() && show_message)
                printf("\033[1A");
        }
    }

    if (count > 0 && show_message)
        std::cout << std::endl;

    threads.clear();
    std::vector<std::string> files;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(Runtime::tmp_dir.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            std::string file = ent->d_name;

            if (file.rfind("_DMIT_tmp", 0) == 0)
            {
                file = Runtime::tmp_dir + "/" + file;
                Package npackage = Package(YAML::LoadFile(file));

                for (int i = 0; i < packages.size(); i++)
                {
                    if (npackage.getGitUser() == packages[i].getGitUser() &&
                        npackage.getServer() == packages[i].getServer() &&
                        npackage.getRepoName() == packages[i].getRepoName())
                    {
                        packages[i] = npackage;
                        std::remove(file.c_str());
                    }
                }
            }
        }

        closedir(dir);
    }

    return packages;
}
