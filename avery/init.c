/* init.c */
/* As part of the Avery project */
/* Created by Maxims Enterprise in 2024 */

#define WHITE 0x07

void clear();
void out(char *str, unsigned int at);

// Main entry point for the kernel
// This function is called by the bootloader
void init() {
    clear();
    out("Avery\n", 0);
    while (1) {
    }
}

void clear() {
    // Getting the video memory address
    char *vidmem = (char *)0xb8000;
    unsigned int i = 0;
    while (i < (80 * 25 * 2)) {
        vidmem[i] = ' ';
        vidmem[i + 1] = WHITE;
        i += 2;
    }
}

void out(char *str, unsigned int at) {
    // Getting the video memory address
    char *vidmem = (char *)0xb8000;
    unsigned int i = 0;

    i = (at * 80 * 2);

    while (*str != 0) {
        if (*str == '\n') {
            at++;
            i = (at * 80 * 2);
            str++;
        } else {
            vidmem[i] = *str;
            vidmem[i + 1] = WHITE;
            i += 2;
            str++;
        }
    }
}
