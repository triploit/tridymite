#ifndef TRIDYMITE_UPDATE_LOCAL_PATH_HPP
#define TRIDYMITE_UPDATE_LOCAL_PATH_HPP

#include <vector>
#include <cli/cli.hpp>

#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

void __argument_update_path(const std::vector<std::string> &parameters, CLI* cli)
{
    std::vector<std::string> a = cli->getParameters("up");
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
    std::vector<Package> installed_packages = tstd::load_package_list("packages.lst");
    std::vector<Package> pkg_configs = DependencyManager::getPackagesConfig(installed_packages);

    for (int i = 0; i < installed_packages.size(); i++)
    {
        installed_packages[i] = Package(YAML::LoadFile(installed_packages[i].getRepoName()+"_"+installed_packages[i].getGitUser()+"/pkg/package.yaml"));

        if (installed_packages[i].getVersion() >= pkg_configs[i].getVersion())
        {
            printf(Translation::get("main.package_up_to_date_skipping").c_str(), tstd::package_to_argument(installed_packages[i]).c_str(), installed_packages[i].getVersion().ToString().c_str());

            installed_packages.erase(installed_packages.begin()+i);
            pkg_configs.erase(pkg_configs.begin()+i);

            i--;
        }
    }

    int i = -1;
    for (Package p : installed_packages)
    {
        i++;
        if (p.getVersion() < DependencyManager::getPackagesConfig({p})[0].getVersion())
        {
            std::string val = tstd::package_to_argument(p);
            std::string destination = p.getRepoName()+"_"+p.getGitUser();
            std::cout << Translation::get("main.downloading", false) << " " << tstd::package_to_argument(p) << "..." << std::endl;

            if(std::filesystem::exists(destination))
            {
                if (Runtime::force || tstd::yn_question(Translation::get("main.file_exists_overwrite", false)))
                {
                    tstd::remove_directory(const_cast<char *>((destination + "/").c_str()));
                    system(std::string("mkdir -p "+destination).c_str());

                    if (tstd::download_file(tstd::create_zip_url(p), destination+"/pkg.zip"))
                        std::cout << Translation::get("main.finished", false) << std::endl;
                    else
                        std::cout << Translation::get("main.package_not_found", false) << std::endl;
                }
            }
            else
            {
                system(std::string("mkdir -p "+destination).c_str());

                if (tstd::download_file(tstd::create_zip_url(p), destination+"/pkg.zip"))
                    std::cout << Translation::get("main.finished", false) << std::endl;
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
        }
        else
            printf(Translation::get("main.package_installed_in_path_skipping").c_str(), tstd::package_to_argument(p).c_str(), p.getVersion().ToString().c_str());
    }
}

#endif //TRIDYMITE_UPDATE_LOCAL_PATH_HPP
