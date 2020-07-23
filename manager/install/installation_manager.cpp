#include "installation_manager.hpp"

#include <curl/curl.h>
#include <yaml-cpp/yaml.h>
#include <sys/stat.h>
#include <unistd.h>

#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif

#include <std/tstd.hpp>
#include <runtime.hpp>
#include <manager/dependencies/dependency_manager.hpp>
#include <algorithm>
#include <manager/remove/remove_manager.hpp>

std::string make_path(const std::string &path)
{
    std::string repl = tstd::replace(path, "$cwd", tstd::get_current_directory());

    if (Runtime::try_local && Runtime::local_folder)
    {
        repl = tstd::replace(repl, "$usr", std::string(getenv("HOME")) + "/.local");
        repl = tstd::replace(repl, "$share", std::string(getenv("HOME")) + "/.local/share");
        repl = tstd::replace(repl, "$bin", std::string(getenv("HOME")) + "/.local/bin");
        repl = tstd::replace(repl, "$lib", std::string(getenv("HOME")) + "/.local/lib");
    }
    else
    {
        repl = tstd::replace(repl, "$usr", "/usr");
        repl = tstd::replace(repl, "$share", "/usr/share");
        repl = tstd::replace(repl, "$bin", "/usr/bin");
        repl = tstd::replace(repl, "$lib", "/usr/lib");
    }

    return repl;
}

bool InstallationManager::linkProducts(const std::string &prefix, const Package &package)
{
    if (package.getLinksTo().size() > 0)
        printf(std::string(prefix+Translation::get("manager.install.linking_files", false)).c_str(),
            package.getLinksTo().size());

    for (int i = 0; i < package.getLinksFrom().size(); i++)
    {
        std::string from = make_path(package.getLinksFrom()[i]);
        std::string to = make_path(package.getLinksTo()[i]);

        to = tstd::replace_quotation_marks(to);
        from = tstd::replace_quotation_marks(from);

        std::string from_var = "_"+std::to_string(Runtime::pid)+"_tridy_pfrom";
        std::string to_var = "_"+std::to_string(Runtime::pid)+"_tridy_pto";

        setenv(from_var.c_str(), from.c_str(), true);
        setenv(to_var.c_str(), to.c_str(), true);

        from_var = "$"+from_var;
        to_var = "$"+to_var;

        std::cout << prefix << Translation::get("manager.install.linking", false) << tstd::trim(package.getProductsTo()[i]) << std::endl;

        if (system(("if [ ! -L "+to_var+" ]; then sudo ln -s "+from_var+" "+to_var+"; fi").c_str()) != 0)
            return false;
    }

    return true;
}

bool InstallationManager::moveProducts(const std::string &prefix, const Package &package)
{
    int count = 0;

    for (const std::string &s : package.getProductsFrom())
        count += tstd::cursive_file_count(s);

    std::cout << prefix;
    if (count != 1)
        printf(Translation::get("manager.install.moving_files").c_str(), count);
    else
        printf(Translation::get("manager.install.moving_file").c_str(), count);

    bool reinstall = IPackagesManager::isPackageInstalled(package);
    bool update = false;

    if (!reinstall)
    {
        if (IPackagesManager::getPackage(package).getVersion() < package.getVersion())
            update = true;
    }

    for (int i = 0; i < package.getProductsFrom().size(); i++)
    {
        if (!Runtime::local_folder && Runtime::try_local)
        {
            std::cout << Translation::get("manager.install.local_directory_not_found", false) << std::endl;

            if (!tstd::yn_question(Translation::get("manager.install.global_installation_question"), false))
            {
                std::cout << Translation::get("manager.install.aborting_installation", false) << std::endl;
                return false;
            }
        }

        std::string from = make_path(tstd::trim(package.getProductsFrom()[i]));
        std::string to = make_path(tstd::trim(package.getProductsTo()[i]));

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

        if (to_exists && !Runtime::force && !Runtime::update && !update)
        {
            if ((to == from ||
                 ((from_file && to_file) ||
                  (!from_file && !to_file)))
                && !reinstall)
            {
                printf(Translation::get("manager.install.file_conflict").c_str(), tstd::package_to_argument(package).c_str(), to.c_str());
                return false;
            }
        }
    }

    for (int i = 0; i < package.getProductsFrom().size(); i++)
    {
        std::string from = make_path(tstd::trim(package.getProductsFrom()[i]));
        std::string to = make_path(tstd::trim(package.getProductsTo()[i]));

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

        std::string from_var = "_"+std::to_string(Runtime::pid)+"_tridy_pfrom";
        std::string to_var = "_"+std::to_string(Runtime::pid)+"_tridy_pto";

        setenv(from_var.c_str(), from.c_str(), true);
        setenv(to_var.c_str(), to.c_str(), true);

        from_var = "$"+from_var;
        to_var = "$"+to_var;

        if (Runtime::force) 
        {
            char buff[Translation::get("manager.install.moving_force").size()+from.size()+to.size()];
            snprintf(buff, sizeof(buff), Translation::get("manager.install.moving_force").c_str(), from.c_str(), to.c_str());
            std::string str = buff;

            std::cout << tstd::replace(str, "=>", "\033[1;32m=>\033[00m");
        }
        else 
        {
            char buff[Translation::get("manager.install.moving").size()+from.size()+to.size()];
            snprintf(buff, sizeof(buff), Translation::get("manager.install.moving").c_str(), from.c_str(), to.c_str());
            std::string str = buff;

            std::cout << tstd::replace(str, "=>", "\033[1;32m=>\033[00m");
        }

        if (!from_exists)
        {
            printf(Translation::get("manager.install.product_not_existing").c_str(), tstd::package_to_argument(package).c_str(), from.c_str());
            return false;
        }

        if (Runtime::verbose)
        {
            std::cout << std::endl;
            std::cout << "[ VERB ] Moving from ";
            system(("echo \\\""+from_var+"\\\"").c_str());

            std::cout << "[ VERB ] Moving to ";
            system(("echo \\\""+to_var+"\\\"").c_str());
        }

        if ((from_file && to_file) ||
            (from_file && !to_file))
        {
            if (system(std::string("if [ -f "+to_var+" ]; then sudo rm "+to_var+"; fi; sudo cp "+from_var+" "+to_var).c_str()) != 0)
            {
                printf(Translation::get("manager.install.moving_error_unknown").c_str(), getenv(from_var.substr(1, from_var.size()).c_str()));
                return false;
            }
        }
        else if (!from_file && to_file)
        {
            printf(Translation::get("manager.install.directory_into_file").c_str(), tstd::package_to_argument(package).c_str(), from.c_str());
            return false;
        }
        else if (!to_file && !from_file)
        {
            if (system(std::string("if [ ! -d "+to_var+" ]; then sudo mkdir -p "+to_var+"; fi; sudo cp -rf "+from_var+"/* "+to_var).c_str()) != 0)
            {
                printf(Translation::get("manager.install.file_neither_exists").c_str(), from.c_str(), from.c_str(), to.c_str());
                return false;
            }
        }
    }

    return true;
}

std::string InstallationManager::downloadPackage(const std::string &prefix, const std::string &package_dir, const std::string &package_zip, const Package &arg)
{
    std::cout << prefix << Translation::get("manager.install.downloading", false) << std::endl;

    if (!tstd::download_file(tstd::create_zip_url(arg), package_zip, true))
    {
        std::cout << Translation::get("general.package_not_found_good", false) << " " << tstd::package_to_argument(arg) << std::endl;
        Runtime::exit(1);
    }

    Runtime::directories_to_clean.push_back(package_dir);
    Runtime::files_to_clean.push_back(package_zip);

    std::cout << prefix << Translation::get("manager.install.unzipping", false) << std::endl;

    mkdir(package_dir.c_str(), 0777);
    chdir(package_dir.c_str());

    if (system(std::string("unzip " + package_zip + " > /dev/null").c_str()) != 0)
    {
        std::cout << Translation::get("manager.install.couldnt_unzip", false) << std::endl;

        if (system("unzip > /dev/null") != 0)
            std::cout << Translation::get("manager.install.install_unzip", false) << std::endl;

        Runtime::exit(1);
    }

    std::string source_path;

    if (Runtime::config["servers"][arg.getServer()]["path"])
        source_path = Runtime::config["servers"][arg.getServer()]["path"].as<std::string>();

    source_path = tstd::trim(tstd::replace_git_vars(source_path, arg, true));

    if (source_path.size() >= 1)
    {
        if (source_path[source_path.size()-1] == '*')
        {
            source_path = source_path.substr(0, source_path.size()-1);

            for (std::string dir : tstd::get_all_directories("./"))
            {
                if (source_path.size() <= dir.size())
                {
                    if (dir.substr(0, source_path.size()) == source_path)
                    {
                        source_path = dir+"/";
                        break;
                    }
                }
            }
        }

        if (source_path[0] == '*')
        {
            source_path = source_path.substr(1, source_path.size());
        }
    }

    for (const auto& dirEntry : std::filesystem::directory_iterator(package_dir))
    {
        if (std::filesystem::is_directory(dirEntry))
        {
            std::string dir_name = dirEntry.path().string();
            std::string pd = package_dir+source_path;
            std::string unformatted_dir_name = dir_name;

            if (dir_name[dir_name.size()-1] != '/' && pd[pd.size()-1] == '/')
            {
                dir_name += "/";
                unformatted_dir_name += "/";
            }

            std::transform(dir_name.begin(), dir_name.end(), dir_name.begin(),
                           [](unsigned char c){ return std::tolower(c); });

            if (dir_name == pd && unformatted_dir_name != dir_name)
            {
                chdir(unformatted_dir_name.c_str());
                return unformatted_dir_name;
            }
        }
    }

    chdir(source_path.c_str());
    return package_dir+source_path;
}

bool InstallationManager::localPackage(std::string path)
{
    std::cout << std::endl;

    if (!tstd::yn_question(Translation::get("general.continue_question", false), true))
    {
        std::cout << Translation::get("general.aborted", false) << std::endl;
        return false;
    }

    if (path[path.size()-1] != '/')
        path += "/";

    std::string file = path+"pkg/package.yaml";
    std::ofstream _of;

    _of.open(file, std::ios_base::app);

    YAML::Node nf = YAML::LoadFile(file);

    if (!nf["gituser"])
    {
        _of << std::endl;
        std::string user;
        std::cout << Translation::get("manager.install.on_which_account", false) << " : " << std::endl;
        std::getline(std::cin, user);

        _of << "gituser: " << user << std::endl;
        std::cout << Translation::get("manager.install.added", false) << std::endl;
    }

    if (!nf["server"])
    {
        std::string server;
        std::cout << Translation::get("manager.install.on_which_server", false) << " : " << std::endl;
        std::getline(std::cin, server);
        _of << "server: " << server << std::endl;
        std::cout << Translation::get("manager.install.added", false) << std::endl;
    }

    if (!nf["reponame"])
    {
        std::string repo;
        std::cout << Translation::get("manager.install.what_name", false) << " : " << std::endl;
        std::getline(std::cin, repo);
        _of << "reponame: " << repo << std::endl;
        std::cout << Translation::get("manager.install.added", false) << std::endl;
    }

    if (!nf["branch"])
    {
        std::string branch;
        std::cout << Translation::get("manager.install.on_which_branch", false) << " : " << std::endl;
        std::getline(std::cin, branch);
        _of << "branch: " << branch << std::endl;
        std::cout << Translation::get("manager.install.added", false) << std::endl;
    }

    _of.close();

    Package package(YAML::LoadFile(file));

    std::cout << Translation::get("manager.install.dependency_searching", false) << std::endl;
    DependencyManager::checkDependencies(package);

    if (IPackagesManager::isPackageInstalled(package))
    {
        if (Runtime::reinstall)
        {
            printf(Translation::get("manager.install.local_reinstall").c_str(), tstd::package_to_argument(package).c_str(), package.getVersion().ToString().c_str());
        }
        else
        {
            printf(Translation::get("manager.install.local_skip").c_str(), tstd::package_to_argument(package).c_str(), package.getVersion().ToString().c_str());
            return false;
        }
    }

    std::string prefix = "\033[1;33m[ " + package.getRepoName() + " ]\033[00m ";
    std::cout << std::endl;
    std::cout << "[ " << Translation::get("manager.install.new_local_installation", false) << " ] " << Translation::get("manager.install.now_installing", false) << " " << tstd::package_to_argument(package) << std::endl;

    chdir(path.c_str());

    std::cout << prefix << Translation::get("manager.install.building", false) << std::endl;

    if (std::ifstream(path+"pkg/package.sh"))
    {
        Script s(Script(path+"pkg/package.sh"));
        s.go();
        s.runFunction("build", package.getGitUser()+"_"+package.getRepoName()+"_"+package.getServer());
    }
    else if (package.getType()["name"])
    {
        if (PreTypeManager::existsPreType(package.getType()["name"].as<std::string>()))
        {
            PreType pre = PreTypeManager::getPreType(package.getType()["name"].as<std::string>());
            std::vector<Variable> variables;

            for (const std::string &s : pre.getNeededVariables())
            {
                if (package.getType()[s])
                    variables.push_back(Variable(s, package.getType()[s].as<std::string>()));
            }

            variables.push_back(Variable("current_directory", tstd::get_current_directory()));
            variables.push_back(Variable("package_name", package.getRepoName()));
            variables.push_back(Variable("package_version", package.getVersion().ToString()));
            variables.push_back(Variable("package_server", package.getServer()));
            variables.push_back(Variable("package_user", package.getGitUser()));

            pre.runScript(variables,
                          package.getGitUser() + "_" + package.getRepoName() + "_" + package.getServer());
        }
        else
        {
            printf(Translation::get("manager.install.pretype_doesnt_exit").c_str(),
                   package.getType()["name"].as<std::string>().c_str());

            if (Runtime::to_install.size() <= 1)
                return false;

            if (!tstd::yn_question(Translation::get("manager.install.skip_and_continue", false), false))
            {
                std::cout << Translation::get("general.aborted", false) << std::endl;
                Runtime::exit(0);
            }

            return false;
        }
    }
    else
    {
        std::cout << Translation::get("manager.install.no_build_script", false) << std::endl;

        if (Runtime::to_install.size() <= 1)
            return false;

        if (!tstd::yn_question(Translation::get("manager.install.skip_and_continue", false), false))
        {
            std::cout << "aborted." << std::endl;
            Runtime::exit(0);
        }

        return false;
    }

    std::cout << prefix << Translation::get("manager.install.installing_version", false) << " " << package.getVersion() << " ..." << std::endl;
    std::string package_str = package.getGitUser()+"+" + package.getRepoName()+"+" + package.getServer();

    if (approveChanges(prefix, package))
    {
        if (InstallationManager::moveProducts(prefix, package))
        {
            if (InstallationManager::linkProducts(prefix, package))
            {
                std::string dir = Runtime::tridy_dir+"/conf/packages/"+package_str;
                system(std::string("if [ ! -d "+dir+" ]; then sudo mkdir -p "+dir+"; fi").c_str());

                system(std::string("sudo cp "+path+"pkg/package.yaml package.yaml.bkp").c_str());

                if (std::ifstream(path+"pkg/package.sh"+dir).is_open())
                    system(std::string("sudo cp "+path+"pkg/package.sh "+dir).c_str());

                std::string c = std::string("sudo echo -e \"\\nlocal: "+std::string(Runtime::try_local ? "true" : "false")+"\\n\" >> "+file+"; sudo cp "+path+"pkg/package.yaml "+dir);
                system(c.c_str());
                system(std::string("sudo cp package.yaml.bkp "+path+"pkg/package.yaml").c_str());

                chdir(Runtime::tmp_dir.c_str());

                Runtime::p_installed++;
                return true;
            }
        }

        std::cout << Translation::get("manager.install.installation_error");
    }

    if (package.getLinksTo().size() > 0 && package.getProductsTo().size() > 0)
    {
        if (tstd::yn_question(Translation::get("manager.install.linking_error_cleanup", false), true))
        {
            RemoveManager::unlinkProducts(prefix, package);
            RemoveManager::removeProducts(prefix, package);
        }
    }

    std::cout << prefix << Translation::get("manager.install.skipping", false) << std::endl;
    return false;
}

bool InstallationManager::installPackage(const Package &arg, bool nl, bool update)
{
    std::string prefix = "\033[1;33m[ " + arg.getRepoName() + " ]\033[00m ";

    if (nl)
        std::cout << std::endl;

    std::cout << "\033[1;33m[ " << Translation::get("manager.install.new_installation", false) << " ]\033[00m " << Translation::get("manager.install.now_installing", false) << " " << tstd::package_to_argument(arg) << std::endl;

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
    _of << "branch: " << arg.getBranch() << std::endl;
    _of.close();

    Package package(YAML::LoadFile(file));

    std::string tridy_dir_backup = Runtime::tridy_dir;
    bool try_local_backup = Runtime::try_local;
    bool supports_local_installation = false;

    if (Runtime::try_local)
    {
        for (const std::string &s : package.getProductsTo())
        {
            if (s.find("$usr") != std::string::npos ||
                s.find("$share") != std::string::npos ||
                s.find("$bin") != std::string::npos ||
                s.find("$lib") != std::string::npos)
                supports_local_installation = true;
        }

        if (!supports_local_installation)
        {
            std::cout << Translation::get("manager.install.doesnt_support_local", false) << std::endl;

            if (!tstd::yn_question(Translation::get("manager.install.global_installation_question", false), false))
            {
                std::cout << Translation::get("general.aborted", false) << std::endl;
                return false;
            }

            Runtime::tridy_dir = Runtime::backup_tridy_dir;
            Runtime::try_local = false;
        }
    }

    std::cout << prefix << Translation::get("manager.install.building", false) << std::endl;

    if (std::ifstream(package_dir+"pkg/package.sh").is_open())
    {
        Script s(Script(package_dir+"pkg/package.sh"));
        s.go();
        s.runFunction("build", arg.getGitUser()+"_"+arg.getRepoName()+"_"+arg.getServer());
    }
    else if (package.getType()["name"])
    {
        if (PreTypeManager::existsPreType(package.getType()["name"].as<std::string>()))
        {
            PreType pre = PreTypeManager::getPreType(package.getType()["name"].as<std::string>());
            std::vector<Variable> variables;

            for (const std::string &s : pre.getNeededVariables())
            {
                if (package.getType()[s])
                    variables.push_back(Variable(s, package.getType()[s].as<std::string>()));
            }

            variables.push_back(Variable("current_directory", tstd::get_current_directory()));
            variables.push_back(Variable("package_name", package.getRepoName()));
            variables.push_back(Variable("package_version", package.getVersion().ToString()));
            variables.push_back(Variable("package_server", package.getServer()));
            variables.push_back(Variable("package_user", package.getGitUser()));

            pre.runScript(variables, arg.getGitUser()+"_"+arg.getRepoName()+"_"+arg.getServer());
        }
        else
        {
            printf(Translation::get("manager.install.pretype_doesnt_exit").c_str(), package.getType()["name"].as<std::string>().c_str());

            if (Runtime::to_install.size() <= 1)
                return false;

            if (!tstd::yn_question(Translation::get("manager.install.skip_and_continue", false), false))
            {
                std::cout << Translation::get("general.aborted", false) << std::endl;
                Runtime::exit(0);
            }

            return false;
        }
    }
    else
    {
        std::cout << Translation::get("manager.install.no_build_script", false) << std::endl;

        if (Runtime::to_install.size() <= 1)
            return false;

        if (!tstd::yn_question(Translation::get("manager.install.skip_and_continue", false), false))
        {
            std::cout << "aborted." << std::endl;
            Runtime::exit(0);
        }

        return false;
    }

    std::cout << prefix << Translation::get("manager.install.installing_version", false) << " " << package.getVersion() << " ..." << std::endl;

    if (approveChanges(prefix, package))
    {
        if (InstallationManager::moveProducts(prefix, package))
        {
            if (InstallationManager::linkProducts(prefix, package))
            {
                std::string dir = Runtime::tridy_dir+"/conf/packages/"+package_str;
                system(std::string("if [ ! -d "+dir+" ]; then sudo mkdir -p "+dir+"; fi").c_str());

                system(std::string("sudo cp "+package_dir+"pkg/package.yaml package.yaml.bkp").c_str());

                if (std::ifstream(package_dir+"pkg/package.sh"+dir).is_open())
                    system(std::string("sudo cp "+package_dir+"pkg/package.sh "+dir).c_str());

                system(std::string("sudo echo -e \"\\nlocal: "+std::string(Runtime::try_local ? "true" : "false")+"\\n\" >> "+file+"; sudo cp "+package_dir+"pkg/package.yaml "+dir).c_str());
                system(std::string("sudo cp package.yaml.bkp "+package_dir+"pkg/package.yaml").c_str());

                chdir(Runtime::tmp_dir.c_str());

                if (update)
                    Runtime::p_updated++;
                else
                    Runtime::p_installed++;

                return true;
            }
        }

        std::cout << Translation::get("manager.install.installation_error");
    }

    if (package.getLinksTo().size() > 0 && package.getProductsTo().size() > 0)
    {
        if (tstd::yn_question(Translation::get("manager.install.linking_error_cleanup", false), true))
        {
            RemoveManager::unlinkProducts(prefix, package);
            RemoveManager::removeProducts(prefix, package);
        }
    }

    if (!supports_local_installation)
    {
        Runtime::tridy_dir = tridy_dir_backup;
        Runtime::try_local = try_local_backup;
    }

    std::cout << prefix << Translation::get("manager.install.skipping", false) << std::endl;
    return false;
}

bool InstallationManager::approveChanges(const std::string &prefix, const Package &package)
{
    int count = 0;
    std::cout << prefix << Translation::get("manager.install.counting_files", false) << std::endl;

    if (package.getProductsFrom().size() > 0)
        std::cout << prefix << Translation::get("manager.install.following_moved", false) << std::endl;

    for (const std::string &s : package.getProductsFrom())
    {
        count += tstd::cursive_file_count(s);

        if (count >= 100)
        {
            std::cout << Translation::get("manager.install.file_warning", false) << std::endl;
            std::cout << prefix << Translation::get("manager.install.continue_counting", false) << std::endl;
        }
    }

    for (int i = 0; i < package.getProductsFrom().size(); i++)
    {
        std::cout << Translation::get("main.arrow", false) <<
                  package.getProductsFrom()[i] <<
                  " ->" <<
                  make_path(package.getProductsTo()[i]) <<
                  std::endl;
    }

    std::cout << std::endl;

    if (package.getLinksFrom().size() != 0)
    {
        std::cout << prefix << Translation::get("manager.install.following_linked", false) << std::endl;

        for (int i = 0; i < package.getLinksFrom().size(); i++)
        {
            std::cout << Translation::get("main.arrow", false) <<
                      package.getLinksFrom()[i] <<
                      " ->" <<
                      make_path(package.getLinksTo()[i]) <<
                      std::endl;
        }

        std::cout << std::endl;

        if (!tstd::yn_question(Translation::get("general.continue_question"), true))
            return false;
    }
    else if (!tstd::yn_question(Translation::get("general.continue_question"), true))
        return false;

    return true;
}
