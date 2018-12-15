#include "package.hpp"

Package::Package()
{
    version = Version("0.0.0a");
}

const std::string &Package::getReponame() const {
    return reponame;
}

void Package::setReponame(const std::string &reponame) {
    Package::reponame = reponame;
}


const std::string &Package::getGituser() const {
    return gituser;
}

void Package::setGituser(const std::string &gituser) {
    Package::gituser = gituser;
}

const std::string &Package::getName() const {
    return name;
}

void Package::setName(const std::string &name) {
    Package::name = name;
}

const std::string &Package::getDescription() const {
    return description;
}

void Package::setDescription(const std::string &description) {
    Package::description = description;
}

const std::string &Package::getInformation() const {
    return information;
}

void Package::setInformation(const std::string &information) {
    Package::information = information;
}

const std::string &Package::getServer() const {
    return server;
}

void Package::setServer(const std::string &server) {
    Package::server = server;
}

const Script &Package::getFunctions() const {
    return functions;
}

void Package::setFunctions(const Script &functions) {
    Package::functions = functions;
}

const std::vector<Package> &Package::getDependencies() const {
    return dependencies;
}

void Package::setDependencies(const std::vector<Package> &dependencies) {
    Package::dependencies = dependencies;
}

const std::vector<std::string> &Package::getAuthors() const {
    return authors;
}

void Package::setAuthors(const std::vector<std::string> &authors) {
    Package::authors = authors;
}

const Version &Package::getVersion() const {
    return version;
}

void Package::setVersion(const Version &version) {
    Package::version = version;
}
