nasm src/boot/boot_sector.asm -f bin -o boot_sector.bin
nasm src/boot/kernel_entry.asm -f elf -o src/boot/kernel_entry.o
nasm src/interrupt_handlers.asm -f elf -o src/interrupt_handlers.o

C_SOURCES=(
    src/kernel
    src/interrupt
    src/driver/port
    src/driver/screen
    src/driver/keyboard
    src/shell
    src/util/string
)

for c_source in ${C_SOURCES[@]}; do
    gcc -m32 -fno-pie -o $c_source.o -fno-stack-protector -ffreestanding -c $c_source.c
done

LINK_COMMAND="ld -m elf_i386 -o kernel.bin -Ttext 0x1000 --oformat binary src/boot/kernel_entry.o src/interrupt_handlers.o"

for c_source in ${C_SOURCES[@]}; do
    LINK_COMMAND="$LINK_COMMAND $c_source.o"
done

$LINK_COMMAND

cat boot_sector.bin kernel.bin > minios.bin
