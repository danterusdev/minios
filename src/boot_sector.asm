[org 0x7c00]
[bits 16]

KERNEL_LOCATION equ 0x1000

mov bp, 0x9000
mov sp, bp

mov bx, KERNEL_LOCATION ; address to write data to
mov ah, 0x02 ; operation: read sector
mov al, 0x10 ; number of sectors
mov ch, 0x00 ; cyllinder 0
mov dh, 0x00 ; head 0
mov cl, 0x02 ; start at sector 2

int 0x13

cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax

jmp CODE_SEG:start_protected_mode

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 0b10011010
    db 0b11001111
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 0b10010010
    db 0b11001111
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

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
