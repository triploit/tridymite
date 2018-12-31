#include "package.hpp"
#include <std/tstd.hpp>

Package::Package()
{
    version = Version("0.0.0a");
}

Package::Package(const std::string &argument_name)
{
    Package p = tstd::parse_package(argument_name);

    setGitUser(p.getGitUser());
    setServer(p.getServer());
    setName(p.getName());
}

const std::string &Package::getGitUser() const {
    return gituser;
}

void Package::setGitUser(const std::string &gituser) {
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

const Script &Package::getBuildScript() const {
    return build_script;
}

void Package::setBuildScript(const Script &functions) {
    Package::build_script = functions;
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
