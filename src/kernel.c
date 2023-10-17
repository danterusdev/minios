char* VIDEO_MEMORY;

void write_string(char* string) {
    int i = 0;
    while (string[i] != 0) {
        VIDEO_MEMORY[i * 2] = string[i];
        i++;
    }
}

void kernel_main() {
    VIDEO_MEMORY = (char*) 0xb8000;

    for (int i = 0; i < 2000; i++) {
        VIDEO_MEMORY[i * 2] = '\0';
    }

    write_string("Hello Kernel!");
}
