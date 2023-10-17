nasm src/boot_sector.asm -f bin -o boot_sector.bin
nasm src/kernel_entry.asm -f elf -o kernel_entry.o
gcc -m32 -fno-pie -o kernel.o -ffreestanding -c src/kernel.c
ld -m elf_i386 -o kernel.bin -Ttext 0x1000 --oformat binary kernel_entry.o kernel.o
cat boot_sector.bin kernel.bin > minios.bin
