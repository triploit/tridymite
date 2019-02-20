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

Package::Package(const YAML::Node &pkg, const std::string &path)
{
    load_package_from_nodes(pkg);
    yaml_path = path;
}

Package::Package(const YAML::Node &pkg)
{
    load_package_from_nodes(pkg);
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
    this->version = version;
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
    if (!p.getName().empty())
        os << "Name        : " << p.getName() << std::endl;

    if (!p.getVersion().str.empty())
        os << std::endl << "Version     : " << p.getVersion() << std::endl;

    if (!p.getDescription().empty())
        os << "Description : " << p.getDescription() << std::endl;

    if (!p.getInformation().empty())
        os << std::endl << "Information : " << p.getInformation() << std::endl;

    if (!p.getAuthors().empty())
    {
        os << std::endl << "Authors     : " << std::endl;

        for (const std::string &s : p.getAuthors())
            os << "    - " << s << std::endl;
    }

    if (!p.getGitUser().empty())
        os << std::endl << "GitUser     : " << p.getGitUser() << std::endl;

    if (!p.getRepoName().empty())
        os << "RepoName    : " << p.getRepoName() << std::endl;

    if (!p.getServer().empty())
        os << "Server      : " << p.getServer() << std::endl;

    return os;
}

const bool Package::operator==(const Package &p) const
{
    return (p.getGitUser() == getGitUser() &&
        p.getServer() == getServer() &&
        p.getRepoName() == getRepoName() &&
        p.getVersion() == getVersion());
}

const std::vector<std::string> &Package::getProductsFrom() const
{
    return products_from;
}

const std::vector<std::string> &Package::getProductsTo() const
{
    return products_to;
}

const std::vector<std::string> &Package::getLinksFrom() const
{
    return links_from;
}

const std::vector<std::string> &Package::getLinksTo() const
{
    return links_to;
}

void Package::setYamlPath(const std::string &path)
{
    yaml_path = path;
}

const std::string &Package::getYamlPath() const
{
    return yaml_path;
}

void Package::load_package_from_nodes(const YAML::Node &pkg)
{
    if (pkg["reponame"])
        setRepoName(pkg["reponame"].as<std::string>());

    if (pkg["gituser"])
        setGitUser(pkg["gituser"].as<std::string>());
    else
    {
        std::cout << "error: package " << getRepoName() << ": no github user set!" << std::endl;
        Runtime::exit(1);
    }

    if (pkg["server"])
        setServer(pkg["server"].as<std::string>());
    else
    {
        std::cout << "error: package " << getRepoName() << ": no server set!" << std::endl;
        Runtime::exit(1);
    }

    if (pkg["name"])
        setName(pkg["name"].as<std::string>());
    else
    {
        std::cout << "error: package " << getRepoName() << ": no name set!" << std::endl;
        Runtime::exit(1);
    }

    if (pkg["description"])
        setDescription(pkg["description"].as<std::string>());

    if (pkg["information"])
        setInformation(pkg["information"].as<std::string>());

    if (pkg["authors"])
        setAuthors(pkg["authors"].as<std::vector<std::string>>());

    if (pkg["version"])
        setVersion(Version(pkg["version"].as<std::string>()));
    else
    {
        std::cout << "error: package " << getRepoName() << ": no version set!" << std::endl;
        Runtime::exit(1);
    }

    if (pkg["products"])
    {
        if (!pkg["products"].IsSequence())
        {
            std::cout << "error: package " << getRepoName() << ": products have to be a sequence!" << std::endl;
            Runtime::exit(1);
        }

        for (YAML::Node n : pkg["products"])
        {
            std::stringstream ss;
            ss << n;
            std::string s = ss.str();

            std::string from = tstd::split(s, ':')[0];
            std::string to = std::string(s).substr(tstd::split(s, ':')[0].size()+1, s.size());

            products_from.push_back(from);
            products_to.push_back(to);
        }
    }

    if (pkg["links"])
    {
        if (!pkg["links"].IsSequence())
        {
            std::cout << "error: package " << getRepoName() << ": links have to be a sequence!" << std::endl;
            Runtime::exit(1);
        }

        for (YAML::Node n : pkg["links"])
        {
            std::stringstream ss;
            ss << n;
            std::string s = ss.str();

            std::string from = tstd::split(s, ':')[0];
            std::string to = std::string(s).substr(tstd::split(s, ':')[0].size()+1, s.size());

            links_from.push_back(from);
            links_to.push_back(to);
        }
    }

    for (const YAML::Node &n : pkg["dependencies"])
    {
        if (n.as<std::string>().rfind("https://", 0) == 0)
        {
            std::string file = Runtime::tmp_dir + "/tmp.yaml";
            std::string url = n.as<std::string>();

            if (!tstd::download_file(url, file))
            {
                std::cout << "error: package " << tstd::package_to_argument(*this) << ": dependency " << url
                          << " seens not to be a tridymite package!" << std::endl;
                Runtime::exit(1);
            }

            std::ofstream _of(file, std::ios::app);
            _of << "gituser: " << this->getGitUser() << std::endl;
            _of << "reponame: " << this->getRepoName() << std::endl;
            _of << "server: " << this->getServer() << std::endl;
            _of.close();

            dependencies.push_back(Package(YAML::LoadFile(file)));
        }
        else if (std::regex_match(n.as<std::string>(), std::regex(
                "([a-zA-Z_0-9\-]*):([a-zA-Z_0-9\-]*)@([a-zA-Z_0-9\-]*)\.([a-zA-Z_0-9\-]*)")))
        {
            dependencies.push_back(tstd::parse_package(n.as<std::string>()));
        }
        else if (n.as<std::string>() == "nopkg")
        {
            std::cout << "error: package " << tstd::package_to_argument(*this)
                      << ": dependencies: nopkg is not allowed yet." << std::endl;
        }
    }
}
