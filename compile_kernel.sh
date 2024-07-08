#!/bin/bash
gcc -Iinc -fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args -c kernel.c -o kernel.o