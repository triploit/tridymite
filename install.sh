#!/usr/bin/env bash

echo "Welcome to the Installer :)!"

if [ -d "/usr/share/tridymite" ]; then
    sleep 1

    echo "Wait... You have tridymite already installed! Are you shure to continue?!"
    printf "[y/n] : "
    read cont

    if [[ "$cont" != "y" ]] && [[ "$cont" != "Y" ]]
    then
        echo "Okay, good :)."
        exit
    fi
fi

if [ -d "yaml-cpp" ]; then
    rm -rf yaml-cpp
fi

if [ -d "tridy_dir" ]; then
    rm -rf tridy_dir
fi

echo "There's also a German language pack. Do you want to activate it?"
printf "[y/n] : "
read german
echo "Okay. Now let's install it!"
echo "."

cp -r pkg tridy_dir
rm tridy_dir/*.sh tridy_dir/*.yaml

if [[ german == "y" ]] || [[ german == "Y" ]]
then
    echo -e "language: \"german\"" > tridy_dir/conf/config.yaml
fi

if [ ! -d /usr/include/yaml-cpp ]; then
    echo "Now installing yaml-cpp from github. It's a dependency of tridymite.";
    wget https://github.com/jbeder/yaml-cpp/archive/yaml-cpp-0.6.2.tar.gz

    tar -xzf yaml-cpp-0.6.2.tar.gz
    cd yaml-cpp*/

    cmake .
    make
    sudo make install
    rm -rf yaml-cpp*
    cd ..
fi

cmake .
make
mv tridymite tridy_dir/tridy

if [ -d "/usr/share/tridymite" ]
then
    echo "Removing an old tridymite directory..."
    sleep 1

    echo "Hey! You have tridymite already installed! Are you really shure to continue?!?!"
    printf "[y/n] : "
    read cont

    if [[ cont == "y" || con == "Y" ]]
    then
        echo "Okay then..."
        sudo rm -rf /usr/share/tridymite
    fi
fi

sudo cp -r tridy_dir /usr/share/tridymite/

sudo rm -rf tridy_dir/
sudo rm -rf yaml-cpp/

echo "And... finished :)"
echo "Have fun!"