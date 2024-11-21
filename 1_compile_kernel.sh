#!/bin/bash

echo "Step 1: Compiling"

mkdir build 2> /dev/null

for file in src/*.c
do
    filename=${file#src/}
    filename=${filename%.c}
    echo "Compiling ${file}"
    gcc -Ideps/gnuefi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c ${file} -o build/${filename}.o
done