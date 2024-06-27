CFLAGS = -ffreestanding -fno-stack-protector -fno-pie -mno-red-zone -m64
LDFLAGS = -nostdlib -Ttext 0x100000

all: kernel.efi

kernel.efi: boot.o kernel.o
    ld $(LDFLAGS) -o kernel.efi boot.o kernel.o

boot.o: boot.s
    as --64 -o boot.o boot.s

kernel.o: kernel.c
    gcc $(CFLAGS) -c -o kernel.o kernel.c

clean:
    rm -f *.o kernel.efi