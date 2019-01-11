#include "package.hpp"

#include <std/tstd.hpp>
#include <yaml-cpp/yaml.h>
#include <regex>

Package::Package()
{
    version = Version("0.0.0a");
}

Package::Package(const std::string &argument_name)
{
    Package p = tstd::parse_package(argument_name);

    setGitUser(p.getGitUser());
    setServer(p.getServer());
    setName(p.getRepoName());
}

Package::Package(const YAML::Node &pkg)
{
    if (pkg["gituser"])
        setGitUser(pkg["gituser"].as<std::string>());

    if (pkg["reponame"])
        setRepoName(pkg["reponame"].as<std::string>());

    if (pkg["server"])
        setServer(pkg["server"].as<std::string>());

    if (pkg["name"])
        setName(pkg["name"].as<std::string>());

    if (pkg["description"])
        setDescription(pkg["description"].as<std::string>());

    if (pkg["information"])
        setInformation(pkg["information"].as<std::string>());

    if (pkg["authors"])
        setAuthors(pkg["authors"].as<std::vector<std::string>>());

    if (pkg["version"])
        setVersion(Version(pkg["version"].as<std::string>()));

    for (const YAML::Node &n : pkg["dependencies"])
    {
        if (n.as<std::string>().rfind("https://", 0) == 0)
        {
            std::cout << "URL" << std::endl;
            // IS URL TO GIT REPOSITORY
        }
        else if (std::regex_match(n.as<std::string>(), std::regex("([a-zA-Z_0-9\-]*):([a-zA-Z_0-9\-]*)@([a-zA-Z_0-9\-]*)\.([a-zA-Z_0-9\-]*)")))
        {
            std::cout << "PACKAGE" << std::endl;
            // IS PACKAGE
        }
        else if (n.as<std::string>() == "nopkg")
        {
            std::cout << "NO_PACKAGE" << std::endl;
            // IS NOT A PACKAGE
            // CAN HAVE TYPES: url, type and arguments (for shell commands as a type)
        }
    }
}

const std::string &Package::getGitUser() const
{
    return gituser;
}

void Package::setGitUser(const std::string &gituser)
{
    Package::gituser = gituser;
}

const std::string &Package::getName() const
{
    return name;
}

void Package::setName(const std::string &name)
{
    Package::name = name;
}

const std::string &Package::getDescription() const
{
    return description;
}

void Package::setDescription(const std::string &description)
{
    Package::description = description;
}

const std::string &Package::getInformation() const
{
    return information;
}

void Package::setInformation(const std::string &information)
{
    Package::information = information;
}

const std::string &Package::getServer() const
{
    return server;
}

void Package::setServer(const std::string &server)
{
    Package::server = server;
}

const Script &Package::getBuildScript() const
{
    return build_script;
}

void Package::setBuildScript(const Script &functions)
{
    Package::build_script = functions;
}

const std::vector<Package> &Package::getDependencies() const
{
    return dependencies;
}

void Package::setDependencies(const std::vector<Package> &dependencies)
{
    Package::dependencies = dependencies;
}

const std::vector<std::string> &Package::getAuthors() const
{
    return authors;
}

void Package::setAuthors(const std::vector<std::string> &authors)
{
    Package::authors = authors;
}

const Version &Package::getVersion() const
{
    return version;
}

void Package::setVersion(const Version &version)
{
    Package::version = version;
}

void Package::setRepoName(const std::string &repo)
{
    repo_name = repo;
}

const std::string &Package::getRepoName() const
{
    return repo_name;
}

std::ostream &operator<<(std::ostream &os, const Package &p)
{
    os << "Name        : " << p.getName() << std::endl;

    os << std::endl << "Version     : " << p.getVersion() << std::endl;
    os << "Description : " << p.getDescription() << std::endl;
    os << std::endl << "Information : " << p.getInformation() << std::endl;

    os << std::endl << "Authors     : " << std::endl;

    for (const std::string &s : p.getAuthors())
        os << "    - " << s << std::endl;

    os << std::endl << "GitUser     : " << p.getGitUser() << std::endl;
    os << "RepoName    : " << p.getRepoName() << std::endl;
    os << "Server      : " << p.getServer() << std::endl;

    return os;
}