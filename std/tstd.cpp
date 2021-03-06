#include <tstd.hpp>
#include <unistd.h>
#include <curl/curl.h>

#include <iostream>
#include <memory>
#include <tgmath.h>
#include <algorithm>
#include <glob.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <iomanip>
#include "tstd.hpp"


std::vector<std::string> tstd::split(std::string s, char delim) // split a string by a delimiter
{
    std::vector<std::string> result;
    std::string tmp = "";

    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == delim)
        {
            result.push_back(tmp);
            tmp = "";
        }
        else
            tmp += s[i];
    }

    if (!tmp.empty())
        result.push_back(tmp);

    return result;
}

std::string tstd::add_prefix(std::string arg) // add a prefix (- or --) to argument (like "help" -> "--help"; "i" -> "-i")
{
    if (arg.size() > 2)
        return "--"+arg;
    return "-"+arg;
}

std::vector<std::string> tstd::get_family(const std::string &arg, const std::vector<std::string> &arg_name) // getting the family of an argument, e.g. "i" is contained in the family of "i,install"
{
    std::vector<std::string> family;

    for (int x = 0; x < arg_name.size(); x++)
    {
        std::string tmp;

        for (char c : arg_name[x])
        {
            if (c == ',')
            {
                family.push_back(tmp);
                tmp = "";
                continue;
            }

            tmp += c;
        }

        if (!tmp.empty())
        {
            family.push_back(tmp);
            tmp = "";
        }

        bool found = false;

        for (std::string s : family)
        {
            if (add_prefix(s) == arg)
                found = true;
        }

        if (found)
            return family;

        family.clear();
    }

    return family;
}

std::string tstd::ltrim(const std::string &str)
{
    std::string s = str;

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
    {
        return !std::isspace(ch);
    }));

    return s;
}

std::string tstd::rtrim(const std::string &str)
{
    std::string s = str;
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());

    return s;
}

std::string tstd::trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

std::string tstd::replace(std::string str, const std::string &from, const std::string &to)
{
    if(from.empty())
        return "";

    size_t start_pos = 0;

    while((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return str;
}

Package tstd::parse_package(const std::string &package)
{
    Package p;
    p.setServer(Runtime::git_server);
    p.setGitUser(Runtime::git_user);
    p.setBranch(Runtime::git_branch);

    bool server = false;
    bool branch = false;
    std::string tmp;

    for (const char &c : package)
    {
        if (c == '#')
        {
            if (server)
            {
                server = false;
                p.setServer(tmp);
            }
            else
                p.setRepoName(tmp);

            tmp = "";
            branch = true;
            continue;
        }

        if (c == '@')
        {
            if (branch)
            {
                branch = false;
                p.setBranch(tmp);
            }
            else
                p.setRepoName(tmp);

            tmp = "";

            server = true;
            continue;
        }

        if (server || branch)
        {
            tmp += c;
            continue;
        }

        if (c == ':')
        {
            p.setGitUser(tmp);
            tmp = "";

            continue;
        }

        tmp += c;
    }

    if (server)
        p.setServer(tmp);
    else if (branch)
        p.setBranch(tmp);
    else
        p.setRepoName(tmp);

    if (p.getGitUser().empty() || p.getRepoName().empty() || p.getServer().empty())
    {
        std::cout << std::endl;
        printf(Translation::get("std.package.package_incomplete").c_str(), package.c_str());
        std::cout << Translation::get("std.package.package_structure", false) << std::endl;
        Runtime::exit(1);
    }

    return p;
}

std::vector<Package> tstd::parse_package_arguments(const std::vector<std::string> &packages)
{
    std::vector<Package> to_ret;

    for (const std::string &s : packages)
    {
        to_ret.push_back(parse_package(s));
    }

    return to_ret;
}

std::string tstd::replace_git_vars(std::string arg, const Package &p, bool replace_tag)
{
    arg = tstd::replace(arg, "$git_user", p.getGitUser());
    arg = tstd::replace(arg, "$git_server", p.getServer());
    arg = tstd::replace(arg, "$git_repository", p.getRepoName());

    if (p.getBranch().size() >= 2)
    {
        if (p.getBranch()[0] == 'v' && (p.getBranch()[1] >= 48 && p.getBranch()[1] <= 57) && replace_tag)
            arg = tstd::replace(arg, "$git_branch", p.getBranch().substr(1, p.getBranch().size()));
        else
            arg = tstd::replace(arg, "$git_branch", p.getBranch());
    }
    else
        arg = tstd::replace(arg, "$git_branch", p.getBranch());

    return arg;
}

std::string tstd::create_zip_url(const Package &p, std::string postfix, const std::string &prefix)
{
    if (!postfix.empty())
        postfix = "/"+postfix;

    if (!Runtime::config["servers"])
    {
        std::cout << Translation::get("std.config.no_servers_defined", false) << std::endl;
        Runtime::exit(1);
    }

    if (!Runtime::config["servers"][p.getServer()] || !Runtime::config["servers"][p.getServer()]["zip"])
    {
        printf(Translation::get("std.config.server_not_defined", false).c_str(), p.getServer().c_str());
        Runtime::exit(1);
    }

    std::string s = Runtime::config["servers"][p.getServer()]["zip"].as<std::string>()+postfix;
    return replace_git_vars(s, p);
}

std::string tstd::create_url(const Package &p, std::string postfix, std::string prefix)
{
    if (!postfix.empty())
        postfix = "/"+postfix;

    return std::string("https://"+prefix+p.getServer()+"/"+p.getGitUser()+"/"+p.getRepoName()+postfix);
}

std::vector<std::string> tstd::read_cursive_all_files(std::string path)
{
    std::vector<std::string> files;

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;

    if (dirp == NULL)
        return files;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        std::string p = path+dp->d_name;
        struct stat s;

        if (stat(p.c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                std::vector<std::string> tmp = read_cursive_all_files(p+"/");
                files.insert(files.end(), tmp.begin(), tmp.end());
            }
            else if (s.st_mode & S_IFREG)
            {
                files.push_back(p);
            }
        }
    }
    closedir(dirp);

    return files;
}

std::vector<std::string> tstd::get_all_directories(const std::string &path)
{
    std::cout.flush();
    std::vector<std::string> dirs;

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        struct stat s;

        if (stat(std::string(path+dp->d_name).c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                dirs.push_back(dp->d_name);
            }
        }
    }

    closedir(dirp);
    return dirs;
}

bool tstd::yn_question(const std::string &q, bool std_yes)
{
    std::cout << q;

    if (!q.empty() && q[q.size()-1] != '\n')
        std::cout << std::endl;

    if (std_yes)
        std::cout << "[Y/n] : ";
    else
        std::cout << "[y/N] : ";

    if (Runtime::insecure_ignore_all_questions)
    {
        std::cout << (std_yes ? "y" : "n") << std::endl;
        return std_yes;
    }

    std::string s;

    while (s != "y" && s != "Y" && s != "n" && s != "N")
    {
        std::getline(std::cin, s);

        if (s != "y" && s != "Y" && s != "n" && s != "N")
        {
            if (s == "")
                return std_yes;

            std::cout << Translation::get("std.neither_y_or_no", false) << std::endl;

            if (std_yes)
                std::cout << "[Y/n] : ";
            else
                std::cout << "[y/N] : ";
        }
    }

    return (s == "Y" || s == "y");
}

void tstd::remove_directory(char *path)
{
    struct dirent *entry = NULL;
    DIR *dir = NULL;
    dir = opendir(path);

    if (dir == NULL)
        return;

    while(entry = readdir(dir))
    {
        DIR *sub_dir = NULL;
        FILE *file = NULL;
        char abs_path[100] = {0};
        if(*(entry->d_name) != '.')
        {
            sprintf(abs_path, "%s/%s", path, entry->d_name);
            if(sub_dir = opendir(abs_path))
            {
                closedir(sub_dir);
                remove_directory(abs_path);
            }
            else
            {
                if(file = fopen(abs_path, "r"))
                {
                    fclose(file);
                    remove(abs_path);
                }
            }
        }
    }

    remove(path);
}

static size_t throw_away(void *ptr, size_t size, size_t nmemb, void *data)
{
    (void)ptr;
    (void)data;
    return (size_t)(size * nmemb);
}

static size_t write_buf(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    tstd::read_buf.append((char*) contents, realsize);
    return realsize;
}

double tstd::check_size(const std::string &url)
{
    CURL *curl = curl_easy_init();
    CURLcode res;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);

        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buf);

        tstd::read_buf.clear();
        res = curl_easy_perform(curl);

        if (tstd::read_buf.find("Status: 404 Not Found") != std::string::npos ||
            tstd::read_buf.find("HTTP/2 404") != std::string::npos ||
            tstd::read_buf.find("HTTP/2 401") != std::string::npos ||
            tstd::read_buf.find("HTTP/1.1 401") != std::string::npos ||
            tstd::read_buf.find("HTTP/1.1 404") != std::string::npos)
            return -2;

        if (!res)
        {
            double filesize;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &filesize);

            if (!res)
            {
                return filesize;
            }
        }
    }

    return -1;
}

int draw_progress_bar(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on

    if (TotalToDownload <= 0.0) {
        return 0;
    }

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    int colums = size.ws_col / 2 - 4;
    int percent = NowDownloaded * 100 / TotalToDownload;

    if (NowDownloaded < 200)
        percent = 0;
    // std::cout << percent << " = " << NowDownloaded << " * 100 / " << TotalToDownload << std::endl;

    float fill = colums * percent / 100;
    int i = 0;

    if (colums < 2)
    {
        std::cout << percent << "%\r";
        return 0;
    }

    std::cout << Translation::get("main.arrow", false) << percent << "% [ ";

    for (; i < fill; i++)
        std::cout << "#";

    for (; i < colums; i++)
        std::cout << "-";

    std::cout << " ] " << std::fixed << std::setprecision(2) <<
        (NowDownloaded / 1024 < 0.05 ? 0 : NowDownloaded / 1024) << "kb";

    printf("\r");
    fflush(stdout);
    return 0;
}

bool tstd::download_file(const std::string &url, const std::string &destination, bool show_progress_bar)
{
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();

    if (curl)
    {
        double r = tstd::check_size(url);

        if (r > 1000000000)
        {
            std::cout << Translation::get("std.big_file", false) << " (" << std::round(r/1000000000) << "GB)." << std::endl;
            if(!tstd::yn_question(Translation::get("general.continue_question", false), false))
            {
                std::cout << Translation::get("general.aborted", false) << std::endl;
                return false;
            }
        }
        else if (r == -2)
            return false;

        fp = fopen(destination.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        if (show_progress_bar)
        {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, draw_progress_bar);
        }

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if (show_progress_bar && ((res >= 200 && res < 300) || res == CURLE_OK))
        {
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
            int colums = size.ws_col / 2 - 4;

            std::cout << " " << Translation::get("main.arrow", false) << "100% [ ";

            for (int i = 0; i < colums; i++)
                std::cout << "#";

            std::cout << " ] " << std::setprecision(2) << std::fixed <<
                (get_file_size(destination) / 1024 < 0.05 ? 0 : get_file_size(destination) / 1024) <<
                "kb       " << std::endl;
        }

        return ((res >= 200 && res < 300) || res == CURLE_OK);
    }

    return false;
}

std::string tstd::package_to_argument(const Package &p, bool no_branch)
{
    if (no_branch)
        return std::string(p.getGitUser()+":"+p.getRepoName()+"@"+p.getServer());
    else
        return std::string(p.getGitUser()+":"+p.getRepoName()+"@"+p.getServer()+"#"+p.getBranch());
}

std::string tstd::exec(const char* cmd)
{
    std::array<char, 128> buffer{};
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

    if (!pipe) throw std::runtime_error("popen() failed!");

    while (!feof(pipe.get()))
    {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}

int tstd::cursive_file_count(const std::string &path, int count)
{
    struct stat info;

    if(stat(path.c_str(), &info) != 0)
    {
        printf(Translation::get("std.object_not_found").c_str(), path.c_str());
        Runtime::exit(1);
    }
    else if( info.st_mode & S_IFDIR)
        count++;
    else
        return 1;

    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;

        std::string p = path+dp->d_name;
        struct stat s;

        if (stat(p.c_str(), &s) == 0)
        {
            if (s.st_mode & S_IFDIR)
            {
                count = cursive_file_count(p, count);
            }
            else if (s.st_mode & S_IFREG)
            {
                count++;
            }
        }
    }
    closedir(dirp);

    return count;
}

std::string tstd::replace_quotation_marks(std::string from)
{
    from = tstd::replace(from, "\\\"", "<¬BSAFZ"+std::to_string(Runtime::pid)+">");
    from = tstd::replace(from, "\"", "\\\"");
    from = tstd::replace(from, "<¬BSAFZ"+std::to_string(Runtime::pid)+">", "\\\"");

    return from;
}

std::vector<std::string> tstd::create_list_of_packages(const std::vector<Package> &packages)
{
    std::vector<std::string> msgs;

    unsigned long spaces_length_uname = 0;
    unsigned long spaces_length_rname = 0;
    unsigned long spaces_length_server = 0;

    for (const Package &installed_package : packages)
    {
        if (spaces_length_uname < installed_package.getGitUser().size())
            spaces_length_uname = installed_package.getGitUser().size();

        if (spaces_length_rname < installed_package.getRepoName().size())
            spaces_length_rname = installed_package.getRepoName().size();

        if (spaces_length_server < installed_package.getServer().size())
            spaces_length_server = installed_package.getServer().size();
    }

    for (const Package &installed_package : packages)
    {
        std::string spaces_uname = "";
        std::string spaces_rname = "";
        std::string spaces_server = "";

        for (int i = 0; i < (spaces_length_uname - installed_package.getGitUser().size()); i++)
        {
            spaces_uname += " ";
        }

        for (int i = 0; i < (spaces_length_rname - installed_package.getRepoName().size()); i++)
        {
            spaces_rname += " ";
        }

        for (int i = 0; i < (spaces_length_server - installed_package.getServer().size()); i++)
        {
            spaces_server += " ";
        }

        if (installed_package.getVersion().ToString().empty())
            msgs.push_back(
                    "    - "+
                    installed_package.getGitUser()+spaces_uname+
                    " -> "+
                    installed_package.getRepoName()+spaces_rname+
                    " @ "+
                    installed_package.getServer()+spaces_server+
                    " #"+
                    installed_package.getBranch());
        else
            msgs.push_back(
                    "    - "+
                    installed_package.getGitUser()+spaces_uname+
                    " -> "+
                    installed_package.getRepoName()+spaces_rname+
                    " @ "+
                    installed_package.getServer()+spaces_server+
                    " - v"+
                    installed_package.getVersion().ToString()+
                    " #"+
                    installed_package.getBranch()); // Add every message to vector
    }

    std::sort(msgs.begin(), msgs.end()); // Sort messages alphabet
    return msgs;
}

std::string tstd::get_current_directory()
{
    char path[MAXPATHLEN];
    getcwd(path, MAXPATHLEN);

    return std::string(path);
}

std::string tstd::get_answer(const std::string &question)
{
    std::cout << question << std::endl;
    std::cout << " > ";

    std::string line;
    std::getline(std::cin, line);

    return line;
}

std::vector<Package> tstd::load_package_list(std::string path)
{
    std::vector<Package> packages;
    std::ifstream f(path);

    if (!f.is_open())
        return {};
    else
    {
        std::string line;

        while (std::getline(f, line))
        {
            if (tstd::trim(path)[0] == '#')
                continue;

            packages.push_back(tstd::parse_package(tstd::trim(line)));
        }
    }

    f.close();
    return packages;
}

void tstd::save_package_list(std::string path, std::vector<Package> packages)
{
    std::ofstream of;
    of.open(path);

    for (Package p : packages)
        of << tstd::package_to_argument(p) << std::endl;

    of.close();
}

std::ifstream::pos_type tstd::get_file_size(std::string path)
{
    std::ifstream in(path, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}
