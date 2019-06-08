function build {
    if [[ `lsb_release -si` == LinuxMint ]]
    then
        if ! grep -qF 's = tstd::replace(s, ": !<!> ", ": ");' package/package.cpp
        then
            echo "Hmm... you are using Linux Mint..."
            echo "We had a bit of trouble with this linux distribution in the past, so we want to"
            echo "prevent you from some errors and add some code, that fixes these errors :) !"

            file_content=`cat package/package.cpp`
            file_content=${file_content/'/*{MINT_LINE}*/'/'s = tstd::replace(s, ": !<!> ", ": ");'}
            echo 'ADDED LINE TO PACKAGE CLASS: s = tstd::replace(s, ": !<!> ", ": ");'

            echo "$file_content" > package/package.cpp
        fi
    fi

    cmake .
    make
}