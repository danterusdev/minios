#include "screen.h"

#include "port.h"

#define VIDEO_MEMORY ((char*) 0xB8000)

#define CURSOR_CONTROL_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

#define CURSOR_CONTROL_HIGH_BYTE 0xE
#define CURSOR_CONTROL_LOW_BYTE 0xF

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

u16 cursor;

void set_character(u16 location, u8 character) {
    VIDEO_MEMORY[location * 2] = character;
}

u8 get_character(u16 location) {
    return VIDEO_MEMORY[location * 2];
}

void set_format(u16 location, u8 format) {
    VIDEO_MEMORY[location * 2 + 1] = format;
}

void check_scroll() {
    if (cursor / SCREEN_WIDTH >= SCREEN_HEIGHT) {
        for (u16 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
            set_character(i, get_character(i + SCREEN_WIDTH));
        }

        cursor -= SCREEN_WIDTH;
    }
}

void kprint(char* message, bool newline) {
    u8 i = 0;
    while (message[i] != '\0') {
        set_character(cursor, message[i]);
        set_format(cursor, 0x0F);
        cursor++;
        i++;
    }

    if (newline) {
        cursor += SCREEN_WIDTH - cursor % SCREEN_WIDTH;
        check_scroll();
    }
    kset_cursor_location(cursor);
}

void kclear() {
    for (u16 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        set_character(i, ' ');
    }
    cursor = 0;
    kset_cursor_location(cursor);
}

void kset_cursor_location(u16 cursor) {
    port_write_byte(CURSOR_CONTROL_PORT, CURSOR_CONTROL_HIGH_BYTE);
    port_write_byte(CURSOR_DATA_PORT, cursor >> 8);

    port_write_byte(CURSOR_CONTROL_PORT, CURSOR_CONTROL_LOW_BYTE);
    port_write_byte(CURSOR_DATA_PORT, cursor & 0xFF);
}
