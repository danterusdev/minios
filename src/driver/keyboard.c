#include "screen.h"
#include "port.h"

#include "../interrupt.h"

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

u8 key_to_char(u16 scancode) {
    switch (scancode) {
        case 0x10:
            return 'q';
        case 0x11:
            return 'w';
        case 0x12:
            return 'e';
        case 0x13:
            return 'r';
        case 0x14:
            return 't';
        case 0x15:
            return 'y';
        case 0x16:
            return 'u';
        case 0x17:
            return 'i';
        case 0x18:
            return 'o';
        case 0x19:
            return 'p';
        case 0x1E:
            return 'a';
        case 0x1F:
            return 's';
        case 0x20:
            return 'd';
        case 0x21:
            return 'f';
        case 0x22:
            return 'g';
        case 0x23:
            return 'h';
        case 0x24:
            return 'j';
        case 0x25:
            return 'k';
        case 0x26:
            return 'l';
        case 0x2C:
            return 'z';
        case 0x2D:
            return 'x';
        case 0x2E:
            return 'c';
        case 0x2F:
            return 'v';
        case 0x30:
            return 'b';
        case 0x31:
            return 'n';
        case 0x32:
            return 'm';
        case 0x39:
            return ' ';
    }
    return '\0';
}

void handle_keyboard() {
    u16 scancode = port_read_byte(KEYBOARD_PORT);
    char character[2];
    character[1] = '\0';

    character[0] = key_to_char(scancode);
    if (scancode == 0x1C) {
        kprint("", true);
    } else if (character[0] != '\0') {
        kprint(character, false);
    }
}

void keyboard_register_handler() {
    register_irq_handler(KEYBOARD_INTERRUPT, handle_keyboard);
    kprint("Initialized keyboard", true);
}
