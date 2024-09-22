#!/bin/bash

echo "Step 2: Linking"

ld -shared -Bsymbolic -Ldeps/gnuefi/x86_64/lib -Ldeps/gnuefi/x86_64/gnuefi -Tdeps/gnuefi/gnuefi/elf_x86_64_efi.lds kernel.o -o main.so