#!/bin/bash

CXX=""
CXX+="g++ -std=c++14 "
CXX+="-g -O0 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors "


build_and_valgrind()
{
    # First argument is build command.
    BUILD_COMMAND=$1

    # Print build command.
    echo $BUILD_COMMAND

    # Execute build command.
    $BUILD_COMMAND

    if [ $? -ne 0 ] ; then
        echo ""
        echo "ERROR: Build failed."
        echo "Build command: $BUILD_COMMAND"
        echo "THE END."
        exit 1
    fi

    valgrind --leak-check=full --track-origins=yes ./a.out > my_test.txt

    if [ $? -ne 0 ] ; then
        echo ""
        echo "ERROR: Valgrind check failed."
        echo "Build command: $BUILD_COMMAND"
        echo "THE END."
        exit 1
    fi
}

build_and_valgrind "$CXX test.cc"

echo ""
echo "All tests finished successfully. The end."