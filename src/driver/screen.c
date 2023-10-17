#include "screen.h"

#include "port.h"

#define VIDEO_MEMORY ((char*) 0xB8000)

#define CURSOR_CONTROL_PORT 0x3D4
#define CURSOR_DATA_PORT 0x3D5

#define CURSOR_CONTROL_HIGH_BYTE 0xE
#define CURSOR_CONTROL_LOW_BYTE 0xF

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 80

u16 cursor;

void kprint(char* message) {
    u8 i = 0;
    while (message[i] != 0) {
        char character = message[i];

        switch (character) {
            case '\n':
                // FIXME: does not work at the very start of a line
                cursor += SCREEN_WIDTH - cursor % SCREEN_WIDTH;
                break;
            default:
                VIDEO_MEMORY[cursor * 2] = message[i];
                VIDEO_MEMORY[cursor * 2 + 1] = 0x0F;
                cursor++;
        }

        i++;
    }
    kset_cursor_location(cursor);
}

void kclear() {
    for (u16 i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        VIDEO_MEMORY[i * 2] = ' ';
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
