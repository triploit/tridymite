#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <string>
#include <vector>

#include "version.hpp"
#include "script.hpp"

#include <yaml-cpp/yaml.h>

class Package
{
private:
    std::string gituser;        // name of user/organisation where the project lies
    std::string repo_name;      // name of the repository
    std::string server;         // name of server (e.g. github.com, bitbucket.org, ...)

    std::string name;           // name of project
    std::string description;    // long description
    std::string information;    // short description
    Script build_script;        // build script

    std::vector<Package> dependencies;  // all dependencies
    std::vector<std::string> authors;   // authors and co-workers

    std::vector<std::string> products_from;
    std::vector<std::string> products_to;

    std::vector<std::string> links_from;
    std::vector<std::string> links_to;

    Version version;

public:
    Package();
    Package(const std::string &argument_name);
    Package(const YAML::Node &pkg);

    const std::string &getGitUser() const;
    const std::string &getName() const;
    const std::string &getDescription() const;
    const std::string &getInformation() const;
    const std::string &getServer() const;
    const Script &getBuildScript() const;
    const Version &getVersion() const;
    const std::string &getRepoName() const;
    const std::vector<Package> &getDependencies() const;
    const std::vector<std::string> &getAuthors() const;
    const std::vector<std::string> &getProductsFrom() const;
    const std::vector<std::string> &getProductsTo() const;

    const std::vector<std::string> &getLinksFrom() const;
    const std::vector<std::string> &getLinksTo() const;

    void setGitUser(const std::string &gituser);
    void setName(const std::string &name);
    void setDescription(const std::string &description);
    void setInformation(const std::string &information);
    void setServer(const std::string &server);
    void setBuildScript(const Script &functions);
    void setDependencies(const std::vector<Package> &dependencies);
    void setAuthors(const std::vector<std::string> &authors);
    void setVersion(const Version &version);
    void setRepoName(const std::string &repo);

    friend std::ostream &operator<<(std::ostream &os, const Package &p);
    const bool operator==(const Package &p);
};
#endif