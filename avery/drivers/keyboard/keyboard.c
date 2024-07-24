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
    char c;

    while ((c = get_ch()) != '\n') {
        if (c == 0) {
            continue;
        }
        *ptr++ = c;
        out_ch(c);
    }
    *ptr = '\0';

    out("\n");
    disable_keyboard();
    return buffer;
}
