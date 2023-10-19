#include "screen.h"
#include "port.h"

#include "../interrupt.h"
#include "../shell.h"

#define KEYBOARD_INTERRUPT 0x21

#define KEYBOARD_PORT 0x60

void to_hex_string(u16 value, char* string) {
    u8 count = 0;
    u16 value_temp = value;
    while (value_temp > 0) {
        value_temp /= 0x10;
        count++;
    }
    
    u8 i = 0;
    while (value > 0) {
        i++;

        char character = value % 0x10;

        value /= 0x10;

        if (character > 9) {
            string[count - i] = 'A' + character - 10;
        } else {
            string[count - i] = '0' + character;
        }
    }
    string[count] = '\0';
}

void handle_keyboard() {
    u16 scancode = port_read_byte(KEYBOARD_PORT);
    shell_handle_key(scancode);
}

void keyboard_register_handler() {
    register_irq_handler(KEYBOARD_INTERRUPT, handle_keyboard);
    kprint("Initialized keyboard", true);
}
