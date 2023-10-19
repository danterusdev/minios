#include "driver/screen.h"
#include "string.h"

static char shell_buffer[128];
static u8 shell_buffer_index;

static u16 shell_prompt_location;

void shell_prompt() {
    kprint("> ", false);
    shell_prompt_location = kget_cursor_location();
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

void shell_submit_command() {
    if (string_equal(shell_buffer, "ping")) {
        kprint("Pong", true);
    } else if (string_equal(shell_buffer, "clear")) {
        kclear();
        kset_cursor_location(0);
    } else {
        kprint("Invalid Command '", false);
        kprint(shell_buffer, false);
        kprint("'", true);
    }
}

void shell_handle_key(u16 scancode) {
    char character = key_to_char(scancode);

    kset_cursor_location(shell_prompt_location);

    if (scancode == 0x0E) {
        if (shell_buffer_index > 0) {
            shell_buffer_index--;
            shell_buffer[shell_buffer_index] = '\0';
        }
    } else if (character != '\0') {
        shell_buffer[shell_buffer_index] = character;
        shell_buffer_index++;
    }

    kprint(shell_buffer, false);

    u16 cursor = kget_cursor_location();

    for (u8 i = 0; i < 4; i++) {
        kprint(" ", false);
    }

    kset_cursor_location(cursor);

    if (scancode == 0x1C) {
        kprint("", true);

        shell_submit_command();

        for (u8 i = 0; i < shell_buffer_index; i++) {
            shell_buffer[i] = '\0';
        }
        shell_buffer_index = 0;

        shell_prompt();
    }
}
