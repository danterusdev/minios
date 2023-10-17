#include "driver/port.h"
#include "interrupt.h"

static IDT_Entry idt[256];
static IDT_Register idtr;

void idt_set_gate(u8 vector, void* subroutine) {
    IDT_Entry* gate = &idt[vector];

    gate->isr_low = (u32) subroutine & 0xFFFF;
    gate->isr_high = (u32) subroutine >> 16;
    gate->kernel_cs = 0x8; // not sure exactly what this does
    gate->attributes = 0x8E; // also not sure what this code means
    gate->reserved = 0;
}

#define PIC1_COMMAND           0x20
#define PIC2_COMMAND           0xA0
#define PIC1_DATA              0x21
#define PIC2_DATA              0xA1

#define ICW1_INIT              0x10
#define ICW1_ICW4              0x10

#define ICW3_MASTER_SLAVE_IRQ2 0b00000100
#define ICW3_SLAVE_IRQ2        0b00000010

#define ICW4_8086              0x01

void remap_pics() {
    // Initialize ports
    port_write_byte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    port_write_byte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // Vector offsets
    port_write_byte(PIC1_DATA, 0x20);
    port_write_byte(PIC2_DATA, 0x28);

    port_write_byte(PIC1_DATA, ICW3_MASTER_SLAVE_IRQ2);
    port_write_byte(PIC2_DATA, ICW3_SLAVE_IRQ2);

    // Set the ports to 8086 mode (for more features)
    port_write_byte(PIC1_DATA, ICW4_8086);
    port_write_byte(PIC2_DATA, ICW4_8086);

    // Restore ports
    port_write_byte(PIC1_DATA, 0x00);
    port_write_byte(PIC2_DATA, 0x00);
}

void load_idt() {
    idt_set_gate(0,  isr0);
    idt_set_gate(1,  isr1);
    idt_set_gate(2,  isr2);
    idt_set_gate(3,  isr3);
    idt_set_gate(4,  isr4);
    idt_set_gate(5,  isr5);
    idt_set_gate(6,  isr6);
    idt_set_gate(7,  isr7);
    idt_set_gate(8,  isr8);
    idt_set_gate(9,  isr9);
    idt_set_gate(10, isr10);
    idt_set_gate(11, isr11);
    idt_set_gate(12, isr12);
    idt_set_gate(13, isr13);
    idt_set_gate(14, isr14);
    idt_set_gate(15, isr15);
    idt_set_gate(16, isr16);
    idt_set_gate(17, isr17);
    idt_set_gate(18, isr18);
    idt_set_gate(19, isr19);
    idt_set_gate(20, isr20);
    idt_set_gate(21, isr21);
    idt_set_gate(22, isr22);
    idt_set_gate(23, isr23);
    idt_set_gate(24, isr24);
    idt_set_gate(25, isr25);
    idt_set_gate(26, isr26);
    idt_set_gate(27, isr27);
    idt_set_gate(28, isr28);
    idt_set_gate(29, isr29);
    idt_set_gate(30, isr30);
    idt_set_gate(31, isr31);
    
    remap_pics();

    idt_set_gate(32, irq0);
    idt_set_gate(33, irq1);
    idt_set_gate(34, irq2);
    idt_set_gate(35, irq3);
    idt_set_gate(36, irq4);
    idt_set_gate(37, irq5);
    idt_set_gate(38, irq6);
    idt_set_gate(39, irq7);
    idt_set_gate(40, irq8);
    idt_set_gate(41, irq9);
    idt_set_gate(42, irq10);
    idt_set_gate(43, irq11);
    idt_set_gate(44, irq12);
    idt_set_gate(45, irq13);
    idt_set_gate(46, irq14);
    idt_set_gate(47, irq15);

    idtr.base = (u32) &idt;
    idtr.limit = sizeof(IDT_Entry) * 256;

    asm volatile ("lidt %0" : : "m"(idtr));
}

typedef struct {
   u32 ds;
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
   u32 int_no, err_code;
   u32 eip, cs, eflags, useresp, ss;
} Registers;

void isr_handler(Registers registers) {
    char* video_memory = (char*) 0xb8000;
    video_memory[0] = 'F';

    asm volatile ("hlt");
}

void irq_handler(Registers registers) {
    if (registers.int_no >= 40) port_write_byte(0xA0, 0x20);
    port_write_byte(0x20, 0x20);
}
