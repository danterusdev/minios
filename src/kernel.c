#include "driver/keyboard.h"
#include "driver/screen.h"
#include "driver/port.h"

#include "interrupt.h"
#include "shell.h"

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
    kprint("Loaded Kernel", true);

    load_idt();
    keyboard_register_handler();
    asm volatile ("sti");

    kprint("Loaded interrupt descriptor table", true);

    shell_prompt();
}
