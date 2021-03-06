#!/usr/bin/env bash

error_level=0
error_packages2=""
error_packages3=""
error_packages4=""
error_packages5=""
error_packages6=""
error_packages7=""

SH_RC=""
LANGUAGE="english"

WRITE_SHELL=0           # write local path to SH_RC
CREATE_LOCAL_PATHS=0    # create local paths
WRITE_LANGUAGE=0

function try_running_message() {
    echo "Try running:"
    echo "   (Ubuntu, Mint, Debian)     - $ sudo apt-get install ${1/\#/ }"
    echo "   (Arch, AntergOS, Manjaro)  - $ sudo pacman -S ${2/\#/ }"
    echo "   (Fedora, RHEL, CentOS)     - $ sudo dnf install ${3/\#/ }"
    echo "       (or)                     $ sudo yum install ${4/\#/ }"
    echo "   (Solus Linux)              - $ sudo eopkg install ${5/\#/ }"
    echo "   (openSUSE)                 - $ sudo zypper in ${6/\#/ }"

    error_packages2="$error_packages2${1/\#/ } "
    error_packages3="$error_packages3${2/\#/ } "
    error_packages4="$error_packages4${3/\#/ } "
    error_packages5="$error_packages5${4/\#/ } "
    error_packages6="$error_packages6${5/\#/ } "
    error_packages7="$error_packages7${6/\#/ } "
}

function check_programs() { # command_name, apt, pacman, dnf, yum, eopkg, zypper
    if ! [ -x "$(command -v $1)" ]; then
        echo "Error: $1 is not installed. Please install it."
        try_running_message $2 $3 $4 $5 $6 $7
        echo ""
        error_level=$((error_level+1))
    fi
}

function ask_local {
    if [[ "$PATH" == *"${pfli}"* ]] && [[ "$1" != "-l" ]] && [[ -d "~/.local/bin" ]]
    then
        printf ""
    else
        echo -e "\e[32;1mDo you want to add a directory to your path to make local installations of packages possible?\e[00m";
        printf "[y/n] : "

        read yn
        _local=true
        echo -e "\e[32;1mOkay.\e[00m"

        if [[ "$yn" == "y" ]] || [[ "$yn" == "Y" ]]
        then
            echo -e "Are you using (1) bash, (2) zsh or (3) something else?"
            printf "> "
            read wsh

            if [[ "$wsh" == "1" ]]
            then
                SH_RC="$HOME/.bashrc"
            elif [[ "$wsh" == "2" ]]
            then
                SH_RC="$HOME/.zshrc"
            else
                echo -e "Then please give me the \e[1mcomplete\e[00m path of the autoload script (for bash it's $HOME/.bashrc, for zsh $HOME/.zshrc)."
                printf "> "
                read SH_RC
            fi

            shrc_c=`cat ${SH_RC}`
            if [[ $shrc_c == *"PATH=\"$HOME/.local/bin/:\$PATH\""* ]];
            then
                echo "Installation okay."
            else
                WRITE_SHELL=1
            fi

            CREATE_LOCAL_PATHS=1
        fi
    fi
}

function get_language {
    echo "Following language packs are installed in `pwd`:"

    count=0
    cur=`pwd`

    cd ./pkg/conf/lang/

    lang_names=()
    lang_count=()

    for filename in *.yaml; do
        ((count+=1))
        echo "${count}.) ${filename/.yaml/}"

        lang_names+=(${filename/.yaml/})
    done

    cd $cur

    echo -e "\e[32;1mWhich language pack do you want to use?\e[00m"
    printf "(1-${count}) : "

    read language

    while (( $language > $count )) || (( $language <= 0 ))
    do
        echo "Wrong input. Numbers available: 1-${count}."
        printf "(1-${count}) : "

        read language
    done

    echo "You chose ${lang_names[(($language-1))]}."
    LANGUAGE="${lang_names[(($language-1))]}"
    WRITE_LANGUAGE=1
    echo -e "\e[32;1mOkay.\e[00m"
}

function get_acces_tokens {
    echo -e "\e[32;1mDo you want to add access tokens for the api of git-servers to search online for packages?\e[00m";
    printf "[y/n] : "

    read yn
    echo -e "\e[32;1mOkay.\e[00m"

    if [[ "$yn" == "y" ]] || [[ "$yn" == "Y" ]]
    then
        echo -e "\e[31;1mNOTE: YOU HAVE TO INSTALL PIP AND THE \"reqeusts\" PACKAGE!\e[00m"
        echo -e "Note: GitHub doesn't need a personal access token."
        echo ""
        echo "Following git-servers are supported:"

        count=0
        cur=`pwd`
        cd ./pkg/conf/servants/

        serv_names=()
        serv_count=()
        serv_already=()

        echo -e "For which do you want to set the access tokens for the api?"
        echo -e "(type \"finished\" if don't want to set any more keys.)"

        for filename in *
        do
            if [[ $filename != *".yaml"* ]] && [[ $filename != *"github"* ]]
            then
                ((count+=1))
                echo "${count}.) ${filename/_servant.py/}"

                serv_names+=(${filename/_servant.py/})
            fi
        done

        cd $cur
        finished=false

        while [[ "$servant" != "finished" ]]
        do
            echo -e ""
            printf "(1-${count}) : "
            read servant

            if [[ "$servant" != "finished" ]]
            then
                while (( $servant > $count )) || (( $servant <= 0 ))
                do
                    echo ""
                    echo "Wrong input. Numbers available: 1-${count}."
                    printf "(1-${count}) : "
                    read servant
                done

                function check_it {
                    for num in "${serv_already[@]}"
                    do
                        if [[ "$num" == "$servant" ]]
                        then
                            echo ""
                            echo "Wrong input. You already had that server."
                            echo -e "(type \"finished\" if don't want to set any more keys.)"

                            printf "(1-${count}) : "
                            read servant

                            check_it
                        elif [[ "$servant" == "finished" ]]
                        then
                            exit
                            finished=true
                        fi
                    done
                }

                if [[ "$finished" == "true" ]]
                then
                    exit
                fi

                check_it

                echo "Please enter the api access token for ${serv_names[((servant-1))]}."
                echo "(type \"cancel\" to cancel this server)"
                printf " > "
                read token

                if [[ "$token" != "cancel" ]]
                then
                    echo -e "${serv_names[((servant-1))]}: \"$token\"" >> "./pkg/conf/servants/_tokens.yaml"
                    serv_already=($servant ${serv_already[@]})

                    echo "Added!"
                fi
            fi
        done

        echo -e "\e[32;1mOkay.\e[00m"
    fi
}

function install_yaml_cpp {
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
}

#########################################################################
#
# CHECKING INSTALLED PROGRAMS
#
#########################################################################

# check_programs name apt pacman dnf yum eopkg zypper

check_programs git git git git-all git-core git git-core
# check_programs curl libcurl4-gnutls-dev curl libcurl-gnutls libcurl-devel libcurl-devel curl libcurl-devel
check_programs wget wget wget wget 'wget#-y' wget
check_programs g++ g++ gcc gcc-c++ gcc-c++ '-c#system.devel' gcc-c++
check_programs make make make make make '-c#system.devel' '-t#devel_basis'
check_programs cmake cmake cmake cmake cmake '-c#system.devel' cmake

if [ ! -f "/usr/include/x86_64-linux-gnu/curl/curl.h" ] && [ ! -f "/usr/include/curl/curl.h" ] && [ ! -f "/usr/include/x86_64-linux-gnu/curl/curl.h" ]
then
    echo "Error: libcurl isn't installed!"
    try_running_message "libcurl4-gnutls-dev" "libcurl-gnutls" "libcurl-devel" "libcurl" "curl-devel" "libcurl-devel"
    error_level=$((error_level+1))
    echo ""
fi

if [ ! -d "/usr/include/yaml-cpp" ] && [ ! -d "/usr/local/include/yaml-cpp" ]
then
    echo "Error: yaml-cpp isn't installed!"
    try_running_message libyaml-cpp-dev yaml-cpp yaml-cpp-devel yaml-cpp-devel yaml-cpp-devel yaml-cpp-devel
    error_level=$((error_level+1))
    echo ""
fi

if [[ $error_level > 0 ]]; # apt pacman dnf yum eopkg zypper
then
    if [[ $error_level > 1 ]]; # apt pacman dnf yum eopkg zypper
    then
        echo "or everything:"
        try_running_message "$error_packages2" "$error_packages3" "$error_packages4" "$error_packages5" "$error_packages6" "$error_packages7"
    fi

    exit 1
fi

# install_yaml_cpp

#########################################################################
#
# END
#
#########################################################################

if [[ `whoami` == "root" ]]
then
    echo "You shouldn't run this installer as root.";
    exit
fi

pfli="${HOME}/.local/bin"
_local=false

echo -e "\e[32;1mWelcome to the Installer :)!\e[00m"

if [ -d "/usr/share/tridymite/conf/packages" ]
then
    mkdir bkp_pkg
    sudo cp -rf /usr/share/tridymite/conf/packages/* bkp_pkg/
fi

if [ -d "/usr/share/tridymite" ]
then
    sleep 1

    echo -e "\e[31;1mWait... You have tridymite already installed! Are you shure to continue?!\e[00m"
    printf "[y/n] : "
    read cont

    if [[ "$cont" != "y" ]] && [[ "$cont" != "Y" ]]
    then
        echo -e "\e[32;1mOkay, good :).\e[00m"
        rm -rf bkp_pkg
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

cp -r pkg tridy_dir
rm tridy_dir/*.sh tridy_dir/*.yaml

#########################################################################
#
# ASKING FOR LANGUAGE
#
#########################################################################

if [[ "$1" != "-a" ]] && [[ "$1" != "-al" ]]; then
    get_language
fi

#########################################################################
#
# ASKING FOR LOCAL MODULE
#
#########################################################################

if [[ "$1" != "-a" ]] && [[ "$1" != "-al" ]]; then
    ask_local
fi

if [[ "$1" == "-al" ]]; then
    if [[ "$PATH" == *"${pfli}"* ]] && [[ -d "~/.local/bin" ]]
    then
        printf ""
    else
        _local=true
        CREATE_LOCAL_PATHS=1
    fi
fi

#########################################################################
#
# ASKING FOR API ACCESS TOKENS
#
#########################################################################

get_acces_tokens

#########################################################################
#
# INSTALLING TRIDYMITE
#
#########################################################################

if [[ `lsb_release -si` == LinuxMint ]] || [[ `lsb_release -si` == Zorin ]]
then
    if ! grep -qF 's = tstd::replace(s, "!<!> ", "");' package/package.cpp
    then
        echo "Hmm... you are using Linux Mint..."
        echo "We had a bit of trouble with this linux distribution in the past, so we want to"
        echo "prevent you from some errors and add some code, that fixes these errors :) !"

        file_content=`cat package/package.cpp`
        GF='"'
        file_content=${file_content/"/*{MINT_LINE}*/"/"s = tstd::replace(s, ${GF}!<!> ${GF}, ${GF}${GF});"}
        file_content=${file_content/"/*{MINT_LINE}*/"/"s = tstd::replace(s, ${GF}!<!> ${GF}, ${GF}${GF});"}
        echo 'ADDED LINE TO PACKAGE CLASS: s = tstd::replace(s, "!<!> ", "");'

        echo "$file_content" # > package/package.cpp
    fi
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

if [[ "$_local" == "true" ]]
then
    mkdir -p ~/.local/share/tridymite/packages/
fi

if [[ $WRITE_SHELL == 1 ]]; then
    echo -e "\nPATH=\"$HOME/.local/bin/:\$PATH\"" >> "${SH_RC}"
fi

if [[ $CREATE_LOCAL_PATHS == 1 ]]; then
    mkdir -p ${HOME}/.local/bin
    mkdir -p ${HOME}/.local/share/tridymite/conf/packages
fi


if [[ $WRITE_LANGUAGE == 1 ]]; then
    echo "language: \"${LANGUAGE}\"" >> tridy_dir/conf/config.yaml
fi

sudo cp -r tridy_dir /usr/share/tridymite/
sudo ln -s /usr/share/tridymite/tridy /usr/bin/tridy

if [ -d "bkp_pkg" ]
then
    sudo cp -rf bkp_pkg/* /usr/share/tridymite/conf/packages/
    sudo rm -rf bkp_pkg
fi

sudo rm -rf tridy_dir/
sudo rm -rf yaml-cpp/

echo -e "\e[32;1mAnd... finished :)\e[00m"
echo -e "\e[32;1mHave fun!\e[00m"