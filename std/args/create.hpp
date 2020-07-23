#ifndef TRIDYMITE_CREATE_HPP
#define TRIDYMITE_CREATE_HPP

#include <vector>
#include <iostream>
#include <runtime.hpp>

#include <std/tstd.hpp>

void __argument_create(const std::vector<std::string> &parameters, CLI* cli)
{
    std::string path = tstd::get_current_directory();

    std::string name = tstd::get_answer(Translation::get("main.create.name", false));
    std::string version = tstd::get_answer(Translation::get("main.create.version", false));
    std::string descritpion = tstd::get_answer(Translation::get("main.create.description", false));

    bool information = tstd::yn_question(Translation::get("main.create.info_file", false), false);
    std::string infos;

    if (information) {
        std::ifstream t(tstd::get_answer(Translation::get("main.create.info_path", false)));

        if (!t.is_open()) {
            std::cout << Translation::get("main.create.info_file_not_found");
            Runtime::exit(1);
        }

        std::string line;

        while (std::getline(t, line)) {
            infos += "   "+line;
        }
    }

    std::vector<std::string> authors;
    std::string tmp;

    while ((tmp = tstd::get_answer(Translation::get("main.create.authors", false))) != "(end)") {
        if (tmp != "(end)")
            authors.push_back(tmp);
    }

    std::cout << std::endl;
    std::cout << Translation::get("main.create.tridy_knows");

    for (const PreType &p : PreTypeManager::getPreTypes()) {
        std::cout << "   - " << p.getName() << std::endl;
    }

    bool type = tstd::yn_question(Translation::get("main.create.one_of_them", false), false);
    std::string type_name;
    std::vector<Variable> type_variables;

    if (type) {
        PreType p;

        while (p.getName() == "none") {
            type_name = tstd::get_answer(Translation::get("main.create.type_name", false));


            for (const PreType &type : PreTypeManager::getPreTypes()) {
                if (type.getName() == type_name) {
                    p = type;
                }
            }

            if (p.getName() == "none") {
                std::cout << Translation::get("main.create.wrong_word");
            }
        }

        for (const std::string &var : p.getNeededVariables()) {
            type_variables.push_back(Variable(var, tstd::get_answer(Translation::get("main.create.enter_value", false)+" \""+var+"\".")));
        }
    }

    std::cout << std::endl;

    bool products = tstd::yn_question(Translation::get("main.create.products_now", false), false);
    std::vector<std::string> products_from;
    std::vector<std::string> products_to;

    if (products) {
        std::string ta = "";
        std::cout << Translation::get("main.create.end_to_end");

        while (ta != "(end)") {
            ta = tstd::get_answer(Translation::get("main.create.file_take_from", false));

            if (ta != "(end)") {
                products_from.push_back(ta);
                products_to.push_back(tstd::get_answer(Translation::get("main.create.file_move_to", false)));
            }

            std::cout << std::endl;
        }
    }

    bool links = tstd::yn_question(Translation::get("main.create.links_now", false), false);
    std::vector<std::string> links_from;
    std::vector<std::string> links_to;

    if (links) {
        std::string ta = "";
        std::cout << Translation::get("main.create.end_to_end");

        while (ta != "(end)") {
            ta = tstd::get_answer(Translation::get("main.create.file_take_from", false));

            if (ta != "(end)") {
                links_from.push_back(ta);
                links_to.push_back(tstd::get_answer(Translation::get("main.create.linking_to", false)));
            }

            std::cout << std::endl;
        }
    }

    std::cout << std::endl << Translation::get("main.create.generating_now");
    bool do_it = true;

    if (std::ifstream("pkg/package.yaml").is_open()) {
        do_it = false;

        if (tstd::yn_question(Translation::get("main.create.already_exists_overwrite", false), false)) {
            if (tstd::yn_question(Translation::get("main.create.sure", false), false)) {
                std::cout << Translation::get("main.create.ok");
                do_it = true;
            }
        }
    }

    if (do_it) {
        std::cout << "Writing..." << std::endl;
        mkdir("pkg", 0777);
        std::ofstream of("pkg/package.yaml");

        if (!of.is_open()) {
            std::cout << Translation::get("main.create.cant_write");
            Runtime::exit(1);
        }

        of << "name: " << "\"" << name << "\"" << std::endl;
        of << "version: " << "\"" << version << "\"" << std::endl;
        of << "description: " << "\"" << descritpion << "\"" << std::endl;

        if (information)
            of << "information: >" << std::endl << infos << std::endl;
        else
            of << "information: \"\"" << std::endl;

        if (authors.size() > 0) {
            of << "authors: " << std::endl;

            for (const std::string &s : authors)
                of << "   - " << s << std::endl;
        }

        if (type) {
            of << std::endl;
            of << "type: " << std::endl;
            of << "    name: " << type_name;

            for (const Variable &v : type_variables) {
                of << "    " << v.getName() << ": \"" << v.getValue() << "\"" << std::endl;
            }
        }


        if (products) {
            of << std::endl;
            of << "products: " << std::endl;

            for (int i = 0; i < products_from.size(); i++) {
                of << "    - " << products_from[i] << ": " << "\"" << products_to[i] << "\"" << std::endl;
            }
        }

        if (links) {
            of << std::endl;
            of << "links: " << std::endl;

            for (int i = 0; i < links_from.size(); i++) {
                of << "    - " << links_from[i] << ": " << "\"" << links_to[i] << "\"" << std::endl;
            }

        }

        of.close();
        std::cout << Translation::get("main.create.finished");
    }

}

#endif
