//
// Created by survari on 24.12.18.
//

#include "installation_manager.hpp"

#include <curl/curl.h>
#include <zlib.h>
#include <zip.h>
#include <yaml-cpp/yaml.h>
#include <sys/stat.h>

#include <std/tstd.hpp>
#include <runtime.hpp>
#include <manager/dependencies/dependency_manager.hpp>

bool InstallationManager::linkProducts(const std::string &prefix, const Package &package)
{
    if (package.getLinksTo().size() > 0)
        std::cout << prefix << "linking files..." << std::endl;

    for (int i = 0; i < package.getLinksFrom().size(); i++)
    {
        std::string from = package.getLinksFrom()[i];

        if (Runtime::try_local && Runtime::local_folder)
        {
            from = tstd::replace(from, "$usr", std::string(getenv("HOME"))+"/.local");
            from = tstd::replace(from, "$share", std::string(getenv("HOME"))+"/.local/share");
            from = tstd::replace(from, "$bin", std::string(getenv("HOME"))+"/.local/bin");
            from = tstd::replace(from, "$lib", std::string(getenv("HOME"))+"/.local/lib");
        }
        else
        {
            from = tstd::replace(from, "$usr", "/usr");
            from = tstd::replace(from, "$share", "/usr/share");
            from = tstd::replace(from, "$bin", "/usr/bin");
            from = tstd::replace(from, "$lib", "/usr/lib");
        }

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

        to = tstd::replace_quotation_marks(to);
        from = tstd::replace_quotation_marks(from);

        std::string from_var = "_"+std::to_string(getpid())+"_tridy_pfrom";
        std::string to_var = "_"+std::to_string(getpid())+"_tridy_pto";

        setenv(from_var.c_str(), from.c_str(), true);
        setenv(to_var.c_str(), to.c_str(), true);

        from_var = "$"+from_var;
        to_var = "$"+to_var;

        std::cout << prefix << "linking " << package.getProductsTo()[i] << std::endl;
        system(std::string("ln -s "+from_var+" "+to_var).c_str());
    }

    return true;
}

bool InstallationManager::moveProducts(const std::string &prefix, const Package &package)
{
    int count = 0;

    std::cout << prefix << "counting files..." << std::endl;
    for (const std::string &s : package.getProductsFrom())
    {
        count += tstd::cursive_file_count(s);

        if (count >= 100)
        {
            std::cout << "warning: this package contains more than 100 files." << std::endl;
            std::cout << prefix << "continue counting..." << std::endl;
        }
    }

    if (count >= 100)
    {
        std::cout << std::endl;

        if (count != 1)
            std::cout << "moving " << count << " files..." << std::endl;
        else
            std::cout << "moving " << count << " file..." << std::endl;

        if (!tstd::yn_question("do you want to continue?"))
            return false;
    }

    bool reinstall = IPackagesManager::isPackageInstalled(package);

    for (int i = 0; i < package.getProductsFrom().size(); i++)
    {
        if (!Runtime::local_folder && Runtime::try_local)
        {
            std::cout << "error: couldn't find the directory for local packages." << std::endl;

            if (!tstd::yn_question("do you want to continue and install it globally?"))
            {
                std::cout << "aborted package installation." << std::endl;
                return 0;
            }
        }

        std::string from = tstd::trim(package.getProductsFrom()[i]);

        if (Runtime::try_local && Runtime::local_folder)
        {
            from = tstd::replace(from, "$usr", std::string(getenv("HOME"))+"/.local");
            from = tstd::replace(from, "$share", std::string(getenv("HOME"))+"/.local/share");
            from = tstd::replace(from, "$bin", std::string(getenv("HOME"))+"/.local/bin");
            from = tstd::replace(from, "$lib", std::string(getenv("HOME"))+"/.local/lib");
        }
        else
        {
            from = tstd::replace(from, "$usr", "/usr");
            from = tstd::replace(from, "$share", "/usr/share");
            from = tstd::replace(from, "$bin", "/usr/bin");
            from = tstd::replace(from, "$lib", "/usr/lib");
        }

        std::string to = tstd::trim(package.getProductsTo()[i]);

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

        bool from_file = false;
        bool to_file = false;

        bool from_exists = true;
        bool to_exists = true;

        struct stat info_from;
        struct stat info_to;

        if(stat(to.c_str(), &info_to) != 0)
            to_exists = false;
        else to_file = (info_to.st_mode & S_IFDIR) == 0;

        if(stat(from.c_str(), &info_from) != 0)
            from_exists = false;
        else from_file = (info_from.st_mode & S_IFDIR) == 0;

        to = tstd::replace_quotation_marks(to);
        from = tstd::replace_quotation_marks(from);

        std::string from_var = "_"+std::to_string(getpid())+"_tridy_pfrom";
        std::string to_var = "_"+std::to_string(getpid())+"_tridy_pto";

        setenv(from_var.c_str(), from.c_str(), true);
        setenv(to_var.c_str(), to.c_str(), true);

        from_var = "$"+from_var;
        to_var = "$"+to_var;

        if (Runtime::force)
            std::cout << prefix << " => moving (FORCE) " << from << " to " << to << std::endl;
        else
            std::cout << prefix << " => moving " << from << " to " << to << std::endl;

        if (!from_exists)
        {
            std::cout << "error: package " << tstd::package_to_argument(package) << ": products: \"" << from << "\" doesn't exists!" << std::endl;
            Runtime::exit(1);
        }

        if (to_exists && !Runtime::force && !Runtime::update)
        {
            if ((to == from ||
                ((from_file && to_file) ||
                (!from_file && !to_file)))
                && !reinstall)
            {
                std::cout << "error: package " << tstd::package_to_argument(package) << ": products: conflict:\n    - \"" << to << "\" already exists in filesystem!" << std::endl;
                Runtime::exit(1);
            }
        }

        if ((from_file && to_file) ||
            (from_file && !to_file))
        {
            if (system(std::string("sudo cp "+from_var+" "+to_var).c_str()) != 0)
            {
                std::cout << "error: moving products: \"" << getenv(from_var.substr(1, from_var.size()).c_str()) << "\": unknown error!" << std::endl;
                Runtime::exit(1);
            }
        }
        else if (!from_file && to_file)
        {
            std::cout << "error: package " << tstd::package_to_argument(package) << ": products: \"" << from << "\": cant move a directory into a file!" << std::endl;
            Runtime::exit(1);
        }
        else if (!to_file && !to_file)
        {
            if (system(std::string("if [ ! -d "+to_var+" ]; then sudo mkdir -p "+to_var+"; fi; sudo mv "+from_var+"/* "+to_var).c_str()) != 0)
            {
                std::cout << "error: moving products: \"" << from << "\": unknown error!" << std::endl;
                Runtime::exit(1);
            }
        }
    }

    return true;
}

std::string InstallationManager::downloadPackage(const std::string &prefix, const std::string &package_dir, const std::string &package_zip, const Package &arg)
{
    std::cout << prefix << "downloading ..." << std::endl;

    if (!tstd::download_file(tstd::create_url(arg, "archive/master.zip"), package_zip))
    {
        std::cout << "error: package not found: " << tstd::package_to_argument(arg) << std::endl;
        Runtime::exit(1);
    }

    Runtime::directories_to_clean.push_back(package_dir);
    Runtime::files_to_clean.push_back(package_zip);

    std::cout << prefix << "unzipping ..." << std::endl;

    mkdir(package_dir.c_str(), 0777);
    chdir(package_dir.c_str());

    if (system(std::string("unzip " + package_zip + " > /dev/null").c_str()) != 0)
    {
        std::cout << "error: couldn't unzip file!" << std::endl;

        if (system("unzip > /dev/null") != 0)
            std::cout << "info: install the unzip tool.";

        Runtime::exit(1);
    }

    chdir(std::string(package_dir+arg.getRepoName()+"-master/").c_str());
    return package_dir+arg.getRepoName()+"-master/";
}

void InstallationManager::localPackage(std::string path)
{
    std::cout << std::endl;

    if (!tstd::yn_question("do you want to continue?"))
    {
        std::cout << "aborted." << std::endl;
        return;
    }

    if (path[path.size()-1] != '/')
        path += "/";

    std::string file = path+"pkg/package.yaml";
    std::ofstream _of;

    _of.open(file, std::ios_base::app);

    if (!YAML::LoadFile(file)["reponame"])
    {
        _of << std::endl;
        std::string user;
        std::cout << "on which git account will it lie? : " << std::endl;
        std::getline(std::cin, user);
        _of << "gituser: " << user << std::endl;
        std::cout << "added!" << std::endl;
    }

    if (!YAML::LoadFile(file)["reponame"])
    {
        std::string server;
        std::cout << "on which git server will it lie? : " << std::endl;
        std::getline(std::cin, server);
        _of << "server: " << server << std::endl;
        std::cout << "added!" << std::endl;
    }

    if (!YAML::LoadFile(file)["reponame"])
    {
        std::string repo;
        std::cout << "what's the name of the repository? : " << std::endl;
        std::getline(std::cin, repo);
        _of << "reponame: " << repo << std::endl;
        std::cout << "added!" << std::endl;
    }

    _of.close();

    Package package(YAML::LoadFile(file));

    std::cout << "info: searching for dependencies..." << std::endl;
    DependencyManager::checkDependencies(package);

    if (IPackagesManager::isPackageInstalled(package))
    {
        if (Runtime::reinstall)
        {
            std::cout << "info: (local) package " << tstd::package_to_argument(package) << " v" << package.getVersion() << " is already installed. reinstalling." << std::endl;
        }
        else
        {
            std::cout << "info: (local) package " << tstd::package_to_argument(package) << " v" << package.getVersion() << " is already installed. skipping." << std::endl;
            return;
        }
    }

    std::string prefix = "[ " + package.getRepoName() + " ] ";
    std::cout << std::endl;
    std::cout << "[ new local installation ] now installing " << tstd::package_to_argument(package) << std::endl;

    chdir(path.c_str());

    std::cout << prefix << "building..." << std::endl;

    Script s(Script(path+"pkg/package.sh"));
    s.go();
    s.runFunction("build", package.getGitUser()+"_"+package.getRepoName()+"_"+package.getServer());

    std::cout << prefix << "installing version " << package.getVersion() << " ..." << std::endl;

    std::string package_str = package.getGitUser()+"+" + package.getRepoName()+"+" + package.getServer();

    if (InstallationManager::moveProducts(prefix, package))
    {
        std::string dir = Runtime::tridy_dir+"/conf/packages/"+package_str;
        system(std::string("if [ ! -d "+dir+" ]; then sudo mkdir -p "+dir+"; fi").c_str());

        system(std::string("sudo cp "+path+"pkg/package.yaml package.yaml.bkp").c_str());
        std::string c = std::string("sudo cp "+path+"pkg/package.sh "+dir+"; sudo echo -e \"\\nlocal: "+(Runtime::try_local ? "true" : "false")+"\\n\" >> "+file+"; sudo cp "+path+"pkg/package.yaml "+dir);
        system(c.c_str());
        system(std::string("sudo cp package.yaml.bkp "+path+"pkg/package.yaml").c_str());

        chdir(Runtime::tmp_dir.c_str());
        return;
    }

    InstallationManager::linkProducts(prefix, package);

    std::cout << prefix << "skipping..." << std::endl;
}

void InstallationManager::installPackage(const Package &arg, bool nl)
{
    std::string prefix = "[ " + arg.getRepoName() + " ] ";

    if (nl)
        std::cout << std::endl;
    std::cout << "[ new installation ] now installing " << tstd::package_to_argument(arg) << std::endl;

    std::string package_str = arg.getGitUser()+"+" + arg.getRepoName()+"+" + arg.getServer();
    std::string package_dir = Runtime::tmp_dir+"/" + package_str+"/";
    std::string package_zip = Runtime::tmp_dir+"/tmp_" + package_str+".zip";

    package_dir = InstallationManager::downloadPackage(prefix, package_dir, package_zip, arg);
    chdir(package_dir.c_str());

    std::string file = package_dir+"pkg/package.yaml";
    std::ofstream _of;

    _of.open(file, std::ios_base::app);
    _of << std::endl;
    _of << "gituser: " << arg.getGitUser() << std::endl;
    _of << "reponame: " << arg.getRepoName() << std::endl;
    _of << "server: " << arg.getServer() << std::endl;
    _of.close();

    Package package(YAML::LoadFile(file));

    std::cout << prefix << "building..." << std::endl;

    Script s(Script(package_dir+"pkg/package.sh"));
    s.go();
    s.runFunction("build", arg.getGitUser()+"_"+arg.getRepoName()+"_"+arg.getServer());

    std::cout << prefix << "installing version " << package.getVersion() << " ..." << std::endl;

    if (InstallationManager::moveProducts(prefix, package))
    {
        InstallationManager::linkProducts(prefix, package);

        std::string dir = Runtime::tridy_dir+"/conf/packages/"+package_str;
        system(std::string("if [ ! -d "+dir+" ]; then sudo mkdir -p "+dir+"; fi").c_str());

        system(std::string("sudo cp "+package_dir+"pkg/package.yaml package.yaml.bkp").c_str());
        std::string c = std::string("sudo cp "+package_dir+"pkg/package.sh "+dir+"; sudo echo -e \"\\nlocal: "+(Runtime::try_local ? "true" : "false")+"\\n\" >> "+file+"; sudo cp "+package_dir+"pkg/package.yaml "+dir);
        system(c.c_str());
        system(std::string("sudo cp package.yaml.bkp "+package_dir+"pkg/package.yaml").c_str());

        chdir(Runtime::tmp_dir.c_str());
        return;
    }

    std::cout << prefix << "skipping..." << std::endl;
}
