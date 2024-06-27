mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/os-y.bin
cp grub.cfg isodir/boot/grub/grub.cfg
cp icon.icns isodir/.VolumeIcon.icns
grub-mkrescue -o os-y.iso isodir