//
// Created by survari on 29.12.18.
//

#ifndef TRIDYMITE_FUN_HPP
#define TRIDYMITE_FUN_HPP

#include <sys/types.h>
#include <sys/stat.h>

#include <std/tstd.hpp>

bool get_yn_answer()
{
    char c;

    tstd::dont_rush(" [y/n] : ");
    std::cin >> c;

    while (c != 'y' && c != 'Y' && c != 'n' && c != 'N')
    {
        tstd::dont_rush("\nHey! Please type 'y' for Yes or 'n' for No :)!\n");
        tstd::dont_rush("Do you want to install me?\n");
        tstd::dont_rush(" [y/n] : ");
        std::cin >> c;
    }

    return (c == 'y' || c == 'Y');
}

int fun_to_your_computer()
{
    struct stat info;
    if( stat("./pkg/", &info) != 0 )
    {
        std::cout << "error: fatal: coudln't find ./pkg/ directory for setup!" << std::endl;
        std::cout << "Run tridymite from the cloned source folder of GitHub to run the setup." << std::endl;
        return 1;
    }

    if (!Runtime::loadLanguagePack("./pkg/conf/lang/english.yaml") &&
        !Runtime::loadLanguagePack("./english.yaml"))
    {
        std::cout << "error: couldn't load local language pack: ./pkg/conf/lang/english.yaml" << std::endl;
        std::cout << "Try putting the english.yaml language file into the folder you are working in currently\nand check if the folder ./pkg/ is not broken." << std::endl;
        return 1;
    }

    tstd::dont_rush("\nSo ... Hi :). You haven't installed me yet.\n");
    tstd::dont_rush("Do you want to install me?\n");

    if (get_yn_answer())
    {
        tstd::dont_rush("\nNice! Okay, let's go through the setup.\n");

        tstd::dont_rush("\nLet us tell a bit about us. You have to know, I come from Germany,\nso I can speak German (and that's why my English is so bad :P).\n");
        tstd::dont_rush("Do you want me to speak German (the entire time, also after the installation)?\n");

        if (get_yn_answer())
        {
            tstd::dont_rush("\nOkay mein Freund, dann mal auf weiter!\n(Lade Sprachdateien neu.)\n");
            Runtime::language = "german";

            if (!Runtime::loadLanguagePack())
            {
                if (!Runtime::loadLanguagePack("./pkg/conf/lang/german.yaml"))
                {
                    if (!Runtime::loadLanguagePack("./german.yaml"))
                    {
                        tstd::dont_rush("Fehler: Sprachdatei nicht gefunden!\n");
                        tstd::dont_rush("\nHast du eine Sprachdatei für Deutsch da?\n");

                        if (get_yn_answer())
                        {
                            while (true)
                            {
                                tstd::dont_rush("Wo liegt sie? (bitte den kompletten Pfad angeben)\n : ");

                                std::string file;
                                std::cin.sync();
                                std::getline(std::cin, file);

                                if (!std::fstream(file).is_open())
                                {
                                    tstd::dont_rush("\nFehler: Konnte auch diese Sprachdatei nicht finden :(\nDas Versuchen wir nochmal, oder?\n");

                                    if (get_yn_answer())
                                        continue;
                                    else
                                    {
                                        tstd::dont_rush("So, let's continue in English :).");
                                        break;
                                    }
                                }

                                if (!Runtime::loadLanguagePack(file))
                                {
                                    tstd::dont_rush(
                                            "\nTut mir leid, aber diese Sprachdatei ist fehlerhaft.\nWillst du eine andere probieren?\n");

                                    if (get_yn_answer())
                                        continue;
                                    else
                                    {
                                        tstd::dont_rush("Then let's continue in English :).\n");
                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            tstd::dont_rush("Okay, then let's continue in English :).\n");
                        }
                    }
                }
                else
                    tstd::dont_rush("(Fertig!)\n");
            }
            else
                tstd::dont_rush("(Fertig!)\n");
        }
        else
            tstd::dont_rush("Okay, let's continue in English :).\n");

        tstd::dont_rush("\n"+Runtime::getSentence("fun.1")+"\n");

        system("mkdir tridymite_dir");
        system("cp -r ./pkg/conf tridymite_dir/conf");
        system("cp tridymite tridymite_dir/tridy");

        tstd::dont_rush("\n"+Runtime::getSentence("fun.3")+"\n");

        std::ofstream of;
        of.open("./tridymite_dir/conf/config.yaml");
        of << "language: \"" << Runtime::language << "\"" << std::endl;
        of.close();

        system("sudo mv tridymite_dir /usr/share/tridymite");
        system("sudo ln /usr/share/tridymite/tridy /usr/bin/tridy");

        tstd::dont_rush(Runtime::getSentence("fun.2"));
    }
    else
    {
        tstd::dont_rush("Okay, maybe the next time!\nBye :)\n");
    }

    return 0;
}

#endif

