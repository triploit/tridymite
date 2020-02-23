#ifndef TRIDYMITE_TSTD_HPP
#define TRIDYMITE_TSTD_HPP

#include <iostream>
#include <vector>
#include <package/package.hpp>
#include <runtime.hpp>
#include <dirent.h>

namespace tstd
{
    static std::string read_buf;
    Package parse_package(const std::string &package);

    std::string add_prefix(std::string arg); // Add a prefix (- or --) to argument (like "help" -> "--help"; "i" -> "-i")
    std::string replace_git_vars(std::string arg, const Package &p, bool replace_tag=false);
    std::string create_url(const Package &p, std::string postfix="", std::string prefix="");
    std::string create_zip_url(const Package &p, std::string postfix="", const std::string &prefix="");
    std::string package_to_argument(const Package &p, bool no_branch=false);
    std::string exec(const char* cmd);

    std::string ltrim(const std::string &str);
    std::string rtrim(const std::string &str);
    std::string trim(const std::string &str);
    std::string get_answer(const std::string &question);
    std::string replace(std::string str, const std::string &from, const std::string &to);
    std::string replace_quotation_marks(std::string from);
    std::string get_current_directory();

    bool download_file(const std::string &url, const std::string &destination);
    bool yn_question(const std::string &q);
    bool url_exists(const std::string &url);
    void remove_directory(char* path);

    int cursive_file_count(const std::string &path, int count=1);
    double check_size(const std::string &url);

    std::vector<Package> parse_package_arguments(const std::vector<std::string> &packages);
    std::vector<std::string> split(std::string s, char delim); // Split a string by a delimiter
    std::vector<std::string> get_family(const std::string &arg, const std::vector<std::string> &arg_name); // Getting the family of an argument, e.g. "i" is contained in the family of "i,install"
    std::vector<std::string> read_cursive_all_files(std::string path);
    std::vector<std::string> get_all_directories(const std::string &path);
    std::vector<std::string> create_list_of_packages(const std::vector<Package> &packages);

    std::vector<Package> load_package_list(std::string path);
    void save_package_list(std::string path, std::vector<Package> packages);
};


#endif //TRIDYMITE_TSTD_HPP
