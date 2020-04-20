#ifndef TRIDYMITE_DOWNLOAD_TO_PATH_HPP
#define TRIDYMITE_DOWNLOAD_TO_PATH_HPP

#include <vector>
#include <cli/cli.hpp>
#include <filesystem>

void __argument_download_to_path(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<std::string> a = cli->getParameters("dp");

    if (a.size() < 2)
    {
        printf(Translation::get("cli.argument.too_few").c_str(), tstd::add_prefix("dp").c_str());
        Runtime::exit(1);
    }

    std::string path = a[0];

    struct stat inf;
    if (stat(path.c_str(), &inf) != 0)
    {
        if (system(std::string("mkdir -p " + path).c_str()) != 0)
        {
            std::cout << Translation::get("main.no_permission_to_create_directory", false) << path << std::endl;
            Runtime::exit(1);
        }
    }

    chdir(path.c_str());
    a.erase(a.begin());
    std::vector<Package> pkgs = tstd::parse_package_arguments(a); // Get all parameters and convert them to packages
    std::vector<Package> installed_packages = tstd::load_package_list("packages.lst");

    int cc = 0;
    for (Package p : pkgs)
    {
        cc++;

        bool f = false;
        for (Package pkg : installed_packages)
        {
            if (pkg.getRepoName() == p.getRepoName() && pkg.getGitUser() == p.getGitUser() && pkg.getServer() == p.getServer())
                f = true;
        }

        if (!f)
            installed_packages.push_back(p);

        std::string val = tstd::package_to_argument(p);
        std::string destination = p.getRepoName()+"_"+p.getGitUser();
        std::cout <<  Translation::get("main.downloading", false) << " " << tstd::package_to_argument(p) << "..." << std::endl;

        struct stat info;
        if(std::filesystem::exists(destination))
        {
            if (Runtime::force || tstd::yn_question(Translation::get("main.file_exists_overwrite", false)))
            {
                tstd::remove_directory(const_cast<char *>((destination + "/").c_str()));
                system(std::string("mkdir -p "+destination).c_str());

                if (tstd::download_file(tstd::create_zip_url(p), destination+"/pkg.zip"))
                    std::cout << Translation::get("main.finished_downloading", false) << std::endl;
                else
                    std::cout << Translation::get("main.package_not_found", false) << std::endl;
            }
        }
        else
        {
            system(std::string("mkdir -p "+destination).c_str());

            if (tstd::download_file(tstd::create_zip_url(p), destination+"/pkg.zip"))
                std::cout << Translation::get("main.finished_downloading", false) << std::endl;
            else
                std::cout << Translation::get("main.package_not_found", false) << std::endl;
        }

        std::string source_path;
        if (Runtime::config["servers"][p.getServer()]["path"])
            source_path = Runtime::config["servers"][p.getServer()]["path"].as<std::string>();
        source_path = tstd::trim(tstd::replace_git_vars(source_path, p, true));
        source_path = destination+"/"+source_path;

        if (system(std::string("unzip -o " + destination + "/pkg.zip -d "+destination+"/ > /dev/null").c_str()) != 0)
        {
            std::cout << Translation::get("manager.install.couldnt_unzip", false) << std::endl;

            if (system("unzip > /dev/null") != 0)
                std::cout << Translation::get("manager.install.install_unzip", false) << std::endl;

            Runtime::exit(1);
        }

        if (system(std::string("mv "+source_path+"* "+destination+"/").c_str()) != 0)
        {
            std::cout << Translation::get("main.no_permission_to_move_directory", false) << path << std::endl;
            Runtime::exit(1);
        }
        else
        {
            std::fstream conf(destination+"/pkg/package.yaml", std::ios::app);
            conf << "gituser: \"" << p.getGitUser() << "\"" << std::endl;
            conf << "server: \"" << p.getServer() << "\"" << std::endl;
            conf << "branch: \"" << p.getBranch() << "\"" << std::endl;
            conf << "reponame: \"" << p.getRepoName() << "\"" << std::endl;
            conf.close();
        }

        std::cout << Translation::get("main.finished", false) << std::endl;

        if (cc < pkgs.size())
            std::cout << std::endl;
    }

    tstd::save_package_list("packages.lst", installed_packages);
}

#endif //TRIDYMITE_DOWNLOAD_TO_PATH_HPP
