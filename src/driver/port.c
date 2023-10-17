#include "port.h"

u8 port_read_byte(u16 port) {
    u8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

void port_write_byte(u16 port, u8 value) {
    asm volatile("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}
