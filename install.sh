#!/usr/bin/env bash

pfli="${HOME}/.local/bin"

echo -e "\e[32;1mWelcome to the Installer :)!\e[00m"

if [ -d "/usr/share/tridymite" ]
then
    sleep 1

    echo -e "\e[31;1mWait... You have tridymite already installed! Are you shure to continue?!\e[00m"
    printf "[y/n] : "
    read cont

    if [[ "$cont" != "y" ]] && [[ "$cont" != "Y" ]]
    then
        echo -e "\e[32;1mOkay, good :).\e[00m"
        exit
    fi
fi

if [ -d "yaml-cpp" ]; then
    rm -rf yaml-cpp
fi

if [ -d "tridy_dir" ]
then
    rm -rf tridy_dir
fi

echo -e "\e[32;1mThere's also a German language pack. Do you want to activate it?\e[00m"
printf "[y/n] : "
read german
echo -e "\e[32;1mOkay.\e[00m"

cp -r pkg tridy_dir
rm tridy_dir/*.sh tridy_dir/*.yaml

if [[ "$german" == "y" ]] || [[ "$german" == "Y" ]]
then
    echo -e "language: \"german\"" > tridy_dir/conf/config.yaml
fi

if [[ "$PATH" == *"${pfli}"* ]]
then
    printf ""
else
    echo -e "\e[32;1mDo you want to add a directory to your path to make local installations of packages possible?\e[00m";
    printf "[y/n] : "

    read yn
    echo -e "\e[32;1mOkay.\e[00m"

    if [[ "$german" == "y" ]] || [[ "$german" == "Y" ]]
    then
        echo -e "Are you using (1) bash, (2) zsh or (3) something else?"
        printf "> "
        read wsh

        if [[ "$wsh" == "1" ]]
        then
            shdir="$HOME/.bashrc"
        elif [[ "$wsh" == "2" ]]
        then
            shdir="$HOME/.zshrc"
        else
            echo -e "Then please give me the \e[1mcomplete\e[00m path of the autoload script (for bash it's $HOME/.bashrc, for zsh $HOME/.zshrc)."
            printf "> "
            read shdir
        fi

        echo -e "PATH=\"$HOME/.local/bin/:\$PATH\"" >> "${shdir}"
    fi
fi

if [ ! -d "/usr/include/yaml-cpp" ] && [ ! -d "/usr/local/include/yaml-cpp" ]
then
    echo -e "\e[32;1mNow installing yaml-cpp from github. It's a dependency of tridymite.\e[00m";
    wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.2.tar.gz

    tar -xzf yaml-cpp-0.6.2.tar.gz
    cd yaml-cpp*/

    cmake .
    make
    sudo make install
    rm -rf yaml-cpp*
    cd ..
fi

if [ ! -d "/usr/include/yaml-cpp" ] && [ ! -d "/usr/local/include/yaml-cpp" ]
then
    echo -e "\e[31;1mAn error happened at installation of yaml-cpp.\e[00m"
    echo -e "\e[31;1mExit\e[00m"
    exit 1
fi

if [ ! -d "/usr/include/curl" ]
then
    echo -e "\e[32;1mNow installing libcurl for your system...\e[00m";

    if [ "$(grep -Ei 'debian|buntu|mint' /etc/*release)" ]
    then
        echo -e "\e[32;1mI think you're on a debian machine. Let's go with apt-get.\e[00m"
        sudo apt-get update
        sudo apt-get install libcurl-dev
    elif [ -f "/usr/bin/pacman" ]
    then
        echo -e "\e[32;1mI think you're on an arch linux machine. Let's go with pacman.\e[00m"
        echo -e "\e[32;1m"
        sudo pacman -Sy curl
    elif [ "$(grep -Ei 'suse|opensuse' /etc/*release)" ]
    then
        echo -e "\e[32;1mI think you're on an suse machine. Let's go with zypper.\e[00m"
        sudo zypper install php5-curl
    elif [ "$(grep -Ei 'Red Hat|redhat|cent' /etc/*release)" ]
    then
        echo -e "\e[32;1mI think you're on a redhat or centos machine. Let's go with yum.\e[00m"
        yum install curl
    else
        echo -e "\e[31;1mHm..., I don't know your distribution.\e[00m"
        echo -e "\e[32;1mInstalling for other linux distribution, from source...\e[00m"

        wget https://curl.haxx.se/download/curl-7.63.0.tar.bz2
        tar xfj curl-7.63.0.tar.bz2
        cd curl-7.63.0

        ./configure --prefix=/usr

        make
        sudo make install
    fi
fi

if [ ! -d "/usr/include/curl" ]
then
    echo -e "\e[31;1mAn error happened at installation of yaml-cpp.\e[00m"
    echo -e "\e[31;1mExit\e[00m"
    exit 1
fi

cmake .
make
sudo cp tridymite tridy_dir/tridy

if [ -d "/usr/share/tridymite" ]
then
    echo -e "\e[32;1mRemoving an old tridymite directory...\e[00m"
    sleep 1

    echo -e "\e[31;1mHey! You have tridymite already installed! Are you really shure to continue?!?!\e[00m"
    printf "[y/n] : "
    read cont

    if [[ "$cont" == "y" || "$cont" == "Y" ]]
    then
        echo -e "\e[32;1mOkay then...\e[00m"
        sudo rm -rf /usr/share/tridymite
    else
        sudo rm -rf tridy_dir
        sudo rm -rf yaml-cpp
        echo -e "\e[31;1mAborted.\e[00m"
        exit
    fi
fi

sudo cp -r tridy_dir /usr/share/tridymite/
sudo ln -s /usr/share/tridymite/tridy /usr/bin/tridy

sudo rm -rf tridy_dir/
sudo rm -rf yaml-cpp/

echo -e "\e[32;1mAnd... finished :)\e[00m"
echo -e "\e[32;1mHave fun!\e[00m"