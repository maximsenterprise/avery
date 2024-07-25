/*
 keyboard.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Functions to manage input
 Copyright (c) 2024 Maxims Enterprise
*/

#include "drivers/keyboard.h"

#include "input.h"
#include "interrupts/irq.h"
#include "output.h"
#include "system/io.h"
#include "utils.h"

char current_char = 0;

#ifdef DEBUG
enum KEYBOARD_LAYOUT current_layout = ESP;
current_layout = kbdes;
current_layout_shift = kbdes_shift;
#else
enum KEYBOARD_LAYOUT current_layout = USA;
unsigned char* current = kbdus;
unsigned char* current_shift = kbdus_shift;
#endif

int shift = 0;

// Set the current char to the one in the buffer
void keyboard_handler(struct regs* r) {
    unsigned char scancode = inb(0x60);
    if (scancode & 0x80) {
        // Key released
        scancode &= 0x7F;
        if (scancode == 42 || scancode == 54) {
            shift = 0;
        }
    } else {
        // Key pressed
        if (scancode == 42 || scancode == 54) {
            shift = 1;
        } else {
            if (shift) {
                current_char = current_shift[scancode];
            } else {
                current_char = current[scancode];
            }
        }
    }
}

void enable_keyboard() { irq_install_handler(1, keyboard_handler); }

void disable_keyboard() { irq_uninstall_handler(1); }

void set_layout(enum KEYBOARD_LAYOUT layout) {
    switch (layout) {
        case USA:
            current = kbdus;
            current_shift = kbdus_shift;
            break;
        case ESP:
            current = kbdes;
            current_shift = kbdes_shift;
            break;
        default:
            break;
    }
}

char get_ch() {
    char c = current_char;
    current_char = 0;
    return c;
}

char* input(const char* prompt) {
    enable_keyboard();
    out((char*)prompt);

    static char buffer[256];
    char* ptr = buffer;
    char* buffer_end = buffer + sizeof(buffer) - 1;  // To prevent overflow
    char c;

    while (1) {
        c = get_ch();

        if (c == '\n') {
            // End input on newline
            break;
        } else if (c == 0x08) {  // Handle backspace
            if (ptr > buffer) {  // Ensure we are not at the start of the buffer
                ptr--;           // Move the pointer back
                // Clear the character on the screen
                out_ch(0x08);  // Move cursor back
                out_ch(' ');   // Overwrite with space
                out_ch(0x08);  // Move cursor back again
            }
        } else if (c >= ' ' && ptr < buffer_end) {
            // Handle valid characters
            *ptr++ = c;
            out_ch(c);  // Output the character to the screen
        }
    }

    *ptr = '\0';  // Null-terminate the string

    out("\n");  // Print newline after input
    disable_keyboard();
    return buffer;
}
