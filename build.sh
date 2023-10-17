nasm src/boot/boot_sector.asm -f bin -o boot_sector.bin
nasm src/boot/kernel_entry.asm -f elf -o src/boot/kernel_entry.o
nasm src/interrupt_handlers.asm -f elf -o src/interrupt_handlers.o
gcc -m32 -fno-pie -o src/kernel.o -ffreestanding -c src/kernel.c
gcc -m32 -fno-pie -o src/interrupt.o -ffreestanding -c src/interrupt.c
gcc -m32 -fno-pie -o src/driver/port.o -ffreestanding -c src/driver/port.c
gcc -m32 -fno-pie -o src/driver/screen.o -ffreestanding -c src/driver/screen.c
ld -m elf_i386 -o kernel.bin -Ttext 0x1000 --oformat binary src/boot/kernel_entry.o src/kernel.o src/interrupt.o src/driver/port.o src/driver/screen.o src/interrupt_handlers.o
cat boot_sector.bin kernel.bin > minios.bin
