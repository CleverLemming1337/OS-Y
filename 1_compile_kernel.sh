#!/bin/bash

echo "Step 1: Compiling"

gcc -Ideps/gnuefi/inc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c kernel.c -o kernel.o