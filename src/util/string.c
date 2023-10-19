#include "string.h"

u8 string_length(char* s) {
    u8 result = 0;
    while (s[result] != '\0') {
        result++;
    }
    return result;
}

bool string_equal(char* a, char* b) {
    u8 a_length = string_length(a);
    u8 b_length = string_length(b);

    if (a_length != b_length) return false;

    for (u8 i = 0; i < a_length; i++) {
        if (a[i] != b[i]) return false;
    }

    return true;
}
