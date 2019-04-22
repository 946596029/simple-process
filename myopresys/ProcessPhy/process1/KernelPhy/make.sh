nasm -I boot/include/ boot/boot.asm -o boot.bin
nasm -I boot/include/ boot/loader.asm -o loader.bin

nasm -f elf -I include/ kernel/kernel.asm -o kernel.o
nasm -f elf -I include/ kernel/syscall.asm -o syscall.o
nasm -f elf lib/string.asm -o string.o 
nasm -f elf lib/kliba.asm -o kliba.o

gcc -m32 -I include/ -c kernel/start.c 	-o start.o
gcc -m32 -I include/ -c kernel/i8259.c 	-o i8259.o
gcc -m32 -I include/ -c kernel/global.c -o global.o
gcc -m32 -I include/ -c kernel/protect.c -o protect.o -fno-stack-protector
gcc -m32 -I include/ -c kernel/main.c -o main.o -fno-stack-protector
gcc -m32 -I include/ -c kernel/clock.c -o clock.o

gcc -m32 -I include/ -c lib/klib.c -o klib.o -fno-stack-protector

ld -m elf_i386 -Ttext 0x30400 -o kernel.bin -s kernel.o string.o kliba.o \
	start.o i8259.o global.o protect.o klib.o main.o clock.o syscall.o

dd if=boot.bin of=a.img count=1 bs=512 conv=notrunc
losetup /dev/loop29 a.img
mount /dev/loop29 /mnt/floppy
cp loader.bin /mnt/floppy
cp kernel.bin /mnt/floppy
umount /mnt/floppy
losetup -d /dev/loop29

rm boot.bin
rm loader.bin

rm kernel.o
rm syscall.o
rm string.o
rm kliba.o

rm start.o
rm i8259.o
rm global.o
rm protect.o
rm main.o
rm clock.o

rm klib.o

rm kernel.bin


