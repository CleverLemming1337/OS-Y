#!/bin/bash
echo "Step 5: Building EFI directory"

rm -r EFI_IMG

mkdir EFI_IMG
mkdir EFI_IMG/EFI
mkdir EFI_IMG/EFI/BOOT

cp main.efi EFI_IMG/EFI/BOOT/BOOTx64.efi

cp icon.icns EFI_IMG/.VolumeIcon.icns

