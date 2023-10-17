#include "driver/screen.h"
#include "driver/port.h"
#include "interrupt.h"

void write_string(char* string) {
    char* video_memory = (char*) 0xb8000;
    int i = 0;
    while (string[i] != 0) {
        video_memory[i * 2] = string[i];
        i++;
    }
}

void kernel_main() {
    kclear();
    kprint("Loaded Kernel\n");

    load_idt();
    asm volatile ("sti");

    kprint("Loaded interrupt descriptor table\n");
}
