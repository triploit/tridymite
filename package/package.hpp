#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <string>
#include <vector>

#include "version.hpp"
#include "script.hpp"

class Package
{
private:
    std::string reponame;
public:
    const std::string &getReponame() const;

    void setReponame(const std::string &reponame);

private:
    std::string gituser;
    std::string name;
    std::string description;
    std::string information;
    std::string server;
    Script functions;

    std::vector<Package> dependencies;
    std::vector<std::string> authors;

    Version version;

public:
    Package();

    const std::string &getGituser() const;
    const std::string &getName() const;
    const std::string &getDescription() const;
    const std::string &getInformation() const;
    const std::string &getServer() const;
    const Script &getFunctions() const;
    const std::vector<Package> &getDependencies() const;
    const std::vector<std::string> &getAuthors() const;
    const Version &getVersion() const;

    void setGituser(const std::string &gituser);
    void setName(const std::string &name);
    void setDescription(const std::string &description);
    void setInformation(const std::string &information);
    void setServer(const std::string &server);
    void setFunctions(const Script &functions);
    void setDependencies(const std::vector<Package> &dependencies);
    void setAuthors(const std::vector<std::string> &authors);
    void setVersion(const Version &version);
};
#endif