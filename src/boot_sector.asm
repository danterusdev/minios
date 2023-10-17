[org 0x7c00]
[bits 16]

KERNEL_LOCATION equ 0x1000

mov bp, 0x9000
mov sp, bp

; Loading the kernel
mov bx, KERNEL_LOCATION
mov ah, 0x02 ; Operation: read sector
mov al, 0x10 ; number of sectors
mov ch, 0x00 ; cyllinder 0
mov dh, 0x00 ; head 0
mov cl, 0x02 ; read starting sector 2
int 0x13

; Switch into 32 bit mode
cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax

jmp CODE_SEG:start_protected_mode

%include "src/gdt.asm"

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    jmp KERNEL_LOCATION

times 510-($-$$) db 0
dw 0xaa55
