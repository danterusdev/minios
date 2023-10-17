KERNEL_LOCATION equ 0x1000
DISK_INTERRUPT equ 0x13
DISK_INTERRUPT_READ_SECTORS equ 0x02

[org 0x7c00]
[bits 16]

; Loading the kernel from disk
mov bx, KERNEL_LOCATION
mov ah, DISK_INTERRUPT_READ_SECTORS
mov al, 0x10 ; sector count (512 bytes per sector)
mov ch, 0x00 ; cyllinder
mov dh, 0x00 ; head
mov cl, 0x02 ; starting sector
int DISK_INTERRUPT

; Switch into 32 bit mode
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0b00000001
mov cr0, eax

jmp CODE_SEG:start_protected_mode

%include "src/boot/gdt.asm"

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0xFFFF
    mov esp, ebp

    jmp KERNEL_LOCATION

times 510-($-$$) db 0
dw 0xAA55
