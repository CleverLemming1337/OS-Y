#!/bin/bash

echo "Step 1 of 5: Assembling Boot"
if ./assembleBoot.sh; then
    echo -e "\033[32mSuccess\033[0m"
else
    echo -e "\033[31mFailure\033[0m"
    exit 1
fi

echo "Step 2 of 5: Compiling Kernel"
if ./compileKernel.sh; then
    echo -e "\033[32mSuccess\033[0m"
else
    echo -e "\033[31mFailure\033[0m"
    exit 1
fi

echo "Step 3 of 5: Linking"
if ./link.sh; then
    echo -e "\033[32mSuccess\033[0m"
else
    echo -e "\033[31mFailure\033[0m"
    exit 1
fi

echo "Step 4 of 5: Verifiying multiboot"
if ./verifyMultiboot.sh; then
    echo -e "\033[32mSuccess\033[0m"
else
    echo -e "\033[31mFailure\033[0m"
    exit 1
fi

echo "Step 5 of 5: Building"
if ./build.sh; then
    echo -e "\033[32mSuccess\033[0m"
else
    echo -e "\033[31mFailure\033[0m"
    exit 1
fi