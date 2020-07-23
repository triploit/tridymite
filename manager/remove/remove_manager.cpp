#include "remove_manager.hpp"
#include <std/tstd.hpp>
#include <runtime.hpp>

bool RemoveManager::unlinkProducts(const std::string &prefix, const Package &package)
{
    if (package.getLinksTo().size() > 0)
        std::cout << prefix << Translation::get("manager.remove.removing_links", false) << std::endl;
    else
        return true;

    if (!Runtime::local_folder && Runtime::try_local)
    {
        std::cout << Translation::get("manager.remove.local_directory_not_found", false).c_str() << std::endl;

        if (!tstd::yn_question(Translation::get("manager.remove.remove_globally_instead", false), false))
        {
            std::cout << Translation::get("general.aborted", false) << std::endl;
            return false;
        }
    }

    for (int i = 0; i < package.getLinksTo().size(); i++)
    {
        std::string to = package.getLinksTo()[i];
        to = tstd::replace(to, "$cwd", tstd::get_current_directory());

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
        std::string to_var = std::to_string(Runtime::pid)+"_tridy_pto";
        setenv(to_var.c_str(), to.c_str(), true);
        to_var = "$"+to_var;

        std::cout << prefix << Translation::get("manager.remove.unlinking", false) << " " << to << std::endl;
        system(std::string("if [ -f "+to_var+" ]; then sudo unlink "+to_var+"; fi").c_str());
    }

    return true;
}

bool RemoveManager::removeProducts(const std::string &prefix, const Package &package)
{
    if (package.getProductsTo().size() > 0)
        std::cout << prefix << Translation::get("manager.remove.removing_products", false) << std::endl;
    else
        return true;

    if (!Runtime::local_folder && Runtime::try_local)
    {
        std::cout << Translation::get("manager.remove.local_directory_not_found", false) << std::endl;

        if (!tstd::yn_question(Translation::get("manager.remove.remove_globally_instead", false), false))
        {
            std::cout << Translation::get("general.aborted", false) << std::endl;
            return false;
        }
    }

    for (int i = 0; i < package.getProductsTo().size(); i++)
    {
        std::string to = package.getProductsTo()[i];
        to = tstd::replace(to, "$cwd", tstd::get_current_directory());

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
        std::string to_var = "_"+std::to_string(Runtime::pid)+"_tridy_pto";
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
    std::string url = tstd::create_url(p, "raw/"+p.getBranch()+"/pkg/package.yaml");

    std::cout << "\033[1;33m[ " << Translation::get("manager.remove.removing", false) << " ]\033[00m " << Translation::get("manager.remove.now_uninstalling", false) << " " << tstd::package_to_argument(p) << std::endl;

    Package package = p;
    YAML::Node node;

    if (IPackagesManager::isPackageInstalled(p))
        node = YAML::LoadFile(IPackagesManager::getPackage(package).getYamlPath());
    else
    {
        if (!tstd::download_file(url, file))
        {
            printf(Translation::get("general.package_not_found").c_str(), tstd::package_to_argument(p).c_str());
            Runtime::exit(1);
        }

        std::ofstream _of(file, std::ios::app);
        _of << "gituser: " << p.getGitUser() << std::endl;
        _of << "reponame: " << p.getRepoName() << std::endl;
        _of << "server: " << p.getServer() << std::endl;
        _of << "branch: " << p.getBranch() << std::endl;
        _of.close();

        node = YAML::LoadFile(file);
    }

    package = Package(node);
    bool local = false;
    bool reset_try_local = false;

    if (node["local"])
        local = node["local"].as<bool>();

    if (!Runtime::try_local)
    {
        reset_try_local = true;
        Runtime::try_local = local;
    }

    if (IPackagesManager::isPackageInstalled(package))
    {
        std::string prefix = "\033[1;33m[ " + package.getRepoName() + " ]\033[00m ";
        std::string dir = Runtime::tridy_dir;

        if (dir[dir.size()-1] != '/')
        {
            dir += "/";
        }

        dir += "conf/packages/"+package.getGitUser()+"+"+package.getRepoName()+"+"+package.getServer();

        unlinkProducts(prefix, package);
        removeProducts(prefix, package);

        std::cout << " \033[1;32m=>\033[00m " << Translation::get("manager.remove.removing_path", false) << " " << dir << std::endl;
        system(std::string("sudo rm -rf "+dir).c_str());

        Runtime::p_removed++;

        if (!(Runtime::to_remove[Runtime::to_remove.size()-1] == p))
            std::cout << std::endl;
    }
    else
    {
        printf(Translation::get("manager.remove.package_not_found").c_str(), tstd::package_to_argument(package).c_str());
    }

    if (reset_try_local)
        Runtime::try_local = false;
}