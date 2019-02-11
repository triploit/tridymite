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
    else
        return true;

    if (!Runtime::local_folder && Runtime::try_local)
    {
        std::cout << "error: couldn't find the directory for local packages." << std::endl;

        if (!tstd::yn_question("do you want to continue and remove it globally?"))
        {
            std::cout << "aborted package installation." << std::endl;
            return false;
        }
    }

    for (int i = 0; i < package.getLinksTo().size(); i++)
    {
        std::string to = package.getLinksTo()[i];

        if (Runtime::try_local && Runtime::local_folder)
        {
            to = tstd::replace(to, "$usr", std::string(getenv("HOME"))+"/.local");
            to = tstd::replace(to, "$share", std::string(getenv("HOME"))+"/.local/share");
            to = tstd::replace(to, "$bin", std::string(getenv("HOME"))+"/.local/bin");
            to = tstd::replace(to, "$lib", std::string(getenv("HOME"))+"/.local/lib");
        }
        else
        {
            to = tstd::replace(to, "$usr", "/usr");
            to = tstd::replace(to, "$share", "/usr/share");
            to = tstd::replace(to, "$bin", "/usr/bin");
            to = tstd::replace(to, "$lib", "/usr/lib");
        }

        to = "_"+tstd::replace_quotation_marks(to);
        std::string to_var = std::to_string(getpid())+"_tridy_pto";
        setenv(to_var.c_str(), to.c_str(), true);
        to_var = "$"+to_var;

        std::cout << prefix << "unlinking " << to << std::endl;
        system(std::string("if [ -f "+to_var+" ]; then sudo unlink "+to_var+"; fi").c_str());
    }

    return true;
}

bool RemoveManager::removeProducts(const std::string &prefix, const Package &package)
{
    if (package.getProductsTo().size() > 0)
        std::cout << prefix << "removing products..." << std::endl;
    else
        return true;

    if (!Runtime::local_folder && Runtime::try_local)
    {
        std::cout << "error: couldn't find the directory for local packages." << std::endl;

        if (!tstd::yn_question("do you want to continue and remove it globally?"))
        {
            std::cout << "aborted of removing the package." << std::endl;
            return false;
        }
    }

    for (int i = 0; i < package.getProductsTo().size(); i++)
    {
        std::string to = package.getProductsTo()[i];

        if (Runtime::try_local && Runtime::local_folder)
        {
            to = tstd::replace(to, "$usr", std::string(getenv("HOME"))+"/.local");
            to = tstd::replace(to, "$share", std::string(getenv("HOME"))+"/.local/share");
            to = tstd::replace(to, "$bin", std::string(getenv("HOME"))+"/.local/bin");
            to = tstd::replace(to, "$lib", std::string(getenv("HOME"))+"/.local/lib");
        }
        else
        {
            to = tstd::replace(to, "$usr", "/usr");
            to = tstd::replace(to, "$share", "/usr/share");
            to = tstd::replace(to, "$bin", "/usr/bin");
            to = tstd::replace(to, "$lib", "/usr/lib");
        }

        to = tstd::replace_quotation_marks(to);
        std::string to_var = "_"+std::to_string(getpid())+"_tridy_pto";
        setenv(to_var.c_str(), to.c_str(), true);
        to_var = "$"+to_var;

        std::cout << prefix << "removing " << to << std::endl;
        system(std::string("sudo rm -rf "+to_var).c_str());
    }

    return true;
}

void RemoveManager::uninstallPackage(const Package &p)
{
    std::string file = Runtime::tmp_dir+"/tmp.yaml";
    std::string url = tstd::create_url(p, "raw/master/pkg/package.yaml");

    std::cout << "[ remove ] now uninstalling " << tstd::package_to_argument(p) << std::endl;

    Package package = p;
    YAML::Node n;

    if (IPackagesManager::isPackageInstalled(p))
        n = YAML::LoadFile(IPackagesManager::getPackage(package).getYamlPath());
    else
    {
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

        n = YAML::LoadFile(file);
    }

    package = Package(n);
    bool local = false;
    bool r = false;

    if (n["local"])
        local = n["local"].as<bool>();

    if (!Runtime::try_local)
    {
        r = true;
        Runtime::try_local = local;
    }

    if (IPackagesManager::isPackageInstalled(package))
    {
        std::string prefix = "[ " + package.getRepoName() + " ] ";
        std::string dir = Runtime::tridy_dir+"conf/packages/"+package.getGitUser()+"+"+package.getRepoName()+"+"+package.getServer();

        unlinkProducts(prefix, package);
        removeProducts(prefix, package);

        std::cout << prefix << "removing " << dir << std::endl;
        system(std::string("sudo rm -rf "+dir).c_str());
    }
    else
    {
        std::cout << "warning: removing: package " << tstd::package_to_argument(package) << " not found. skipping." << std::endl;
    }

    if (r)
        Runtime::try_local = false;
}