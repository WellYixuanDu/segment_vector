#!/bin/bash

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

    valgrind --leak-check=full --track-origins=yes $BUILD_COMMAND >> my_test.txt

    if [ $? -ne 0 ] ; then
        echo ""
        echo "ERROR: Valgrind check failed."
        echo "Build command: $BUILD_COMMAND"
        echo "THE END."
        exit 1
    fi
}

if [ -d build ]; then
    cd build
else
    mkdir build && cd build
fi

cmake ..
make
build_and_valgrind "./acc_test"
build_and_valgrind "./speed_test"
echo ""
echo "All tests finished successfully. The end."