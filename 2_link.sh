#!/bin/bash

echo "Step 2: Linking"

ld -shared -Bsymbolic -Lgnuefi/x86_64/lib -Lgnuefi/x86_64/gnuefi -Tgnuefi/gnuefi/elf_x86_64_efi.lds gnuefi/x86_64/gnuefi/crt0-efi-x86_64.o kernel.o -o main.so -lgnuefi -lefi