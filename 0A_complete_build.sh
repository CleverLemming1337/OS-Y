#!/bin/bash

sudo chmod +x 1_compile_kernel.sh
sudo chmod +x 2_link.sh
sudo chmod +x 3_create_efi.sh
sudo chmod +x 5_build_EFIdir.sh

./1_compile_kernel.sh           &&
./2_link.sh                     &&
./3_create_efi.sh               &&
echo "Skipping Step 4: Signing" &&
./5_build_EFIdir.sh