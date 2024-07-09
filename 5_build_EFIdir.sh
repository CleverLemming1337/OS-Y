#!/bin/bash

rm -r EFI_IMG

mkdir EFI_IMG
mkdir EFI_IMG/EFI
mkdir EFI_IMG/EFI/BOOT

cp signed.efi EFI_IMG/EFI/BOOT/BOOTx64.efi

cp icon.icns EFI_IMG/.VolumeIcon.icns