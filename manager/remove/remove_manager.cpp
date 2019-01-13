//
// Created by survari on 24.12.18.
//

#include "remove_manager.hpp"
#include <std/tstd.hpp>
#include <runtime.hpp>

bool RemoveManager::unlinkProducts(const std::string &prefix, const Package &package)
{
    if (package.getLinksTo().size() > 0)
        std::cout << prefix << "removing links..." << std::endl;

    for (int i = 0; i < package.getLinksTo().size(); i++)
    {
        std::cout << prefix << "unlinking " << package.getLinksTo()[i] << std::endl;
        system(std::string("if [ -f "+package.getLinksTo()[i]+" ]; then sudo unlink "+package.getLinksTo()[i]+"; fi").c_str());
    }

    return true;
}

bool RemoveManager::removeProducts(const std::string &prefix, const Package &package)
{
    if (package.getProductsTo().size() > 0)
        std::cout << prefix << "removing products..." << std::endl;

    for (int i = 0; i < package.getProductsTo().size(); i++)
    {
        std::cout << prefix << "removing " << package.getProductsTo()[i] << std::endl;
        system(std::string("sudo rm -rf "+package.getProductsTo()[i]).c_str());
    }

    return true;
}

void RemoveManager::uninstallPackage(const Package &p)
{
    std::string file = Runtime::tmp_dir+"/tmp.yaml";
    std::string url = tstd::create_url(p, "raw/master/pkg/package.yaml");

    if (!tstd::download_file(url, file))
    {
        std::cout << "error: package " << tstd::package_to_argument(p) << " not found!" << std::endl;
        Runtime::exit(1);
    }

    std::ofstream _of(file, std::ios::app);
    _of << "gituser: " << p.getGitUser() << std::endl;
    _of << "reponame: " << p.getRepoName() << std::endl;
    _of << "server: " << p.getServer() << std::endl;
    _of.close();

    Package package(YAML::LoadFile(file));
    if (IPackagesManager::isPackageInstalled(package))
    {
        std::string prefix = "[ " + package.getRepoName() + " ] ";
        std::string dir = Runtime::tridy_dir+"/conf/packages/"+package.getGitUser()+"+"+package.getRepoName()+"+"+package.getServer();

        unlinkProducts(prefix, package);
        removeProducts(prefix, package);

        std::cout << prefix << "removing " << dir << std::endl;
        system(std::string("sudo rm -rf "+dir).c_str());
    }
    else
    {
        std::cout << "warning: removing: package " << tstd::package_to_argument(package) << " not found. skipping." << std::endl;
    }
}