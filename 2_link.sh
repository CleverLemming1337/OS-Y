#!/bin/bash

echo "Step 2: Linking"

ld -shared -Bsymbolic -Ldeps/gnuefi/x86_64/lib -Ldeps/gnuefi/x86_64/gnuefi -Tdeps/gnuefi/gnuefi/elf_x86_64_efi.lds deps/gnuefi/x86_64/gnuefi/crt0-efi-x86_64.o kernel.o -o main.so -lgnuefi -lefi