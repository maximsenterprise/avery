/*
 vga.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Drivers for standard VGA
 Copyright (c) 2024 Maxims Enterprise
*/

#include "vga.h"

#include "output.h"
#include "system/io.h"
#include "utils.h"

// Global variables defined in vga.h
unsigned short *textmemoryptr;
unsigned int cursor_x, cursor_y = 0;
unsigned int color = 0x0F;

// Scroll the screen when needed
void scroll() {
    unsigned blank, temporary;

    blank = 0x20 | (color << 8);

    // Check if we need to scroll
    if (cursor_y >= 25) {
        // Move parts from the screen up until we get to the desired position
        temporary = cursor_y - 25 + 1;
        memory_copy((unsigned char *)textmemoryptr,
                    (unsigned char *)textmemoryptr + temporary * 80 * 2,
                    (25 - temporary) * 80 * 2);

        memory_setw((unsigned char *)textmemoryptr + (25 - temporary) * 80 * 2,
                    blank, 80);
        cursor_y = 25 - 1;
    }
}

void move_cursor() {
    unsigned temporary;

    // Formula for calculating the index
    // Index = [(y * width) + x]
    temporary = cursor_y * 80 + cursor_x;

    // Send the command to the VGA controller
    // 0x3D4 is the index of the command port
    // 0x3D5 is the index of the data port
    outb(0x3D4, 14);
    outb(0x3D5, temporary >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, temporary);
}

void clear() {
    unsigned blank;
    int i;

    blank = 0x20 | (color << 8);

    // Set the entire screen to the blank character
    for (i = 0; i < 80 * 25; i++) {
        textmemoryptr[i] = blank;
    }

    // Move the cursor back to the top left
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void out_ch(unsigned char c) {
    unsigned short *pos;
    unsigned short attr = color << 8;

    // Let's handle a lot of the possible characters and escape sequences

    // Backspace
    if (c == 0x08) {
        if (cursor_x != 0) cursor_x--;
    }
    // Tab
    // We move the cursor to the right by (TAB_WIDTH) spaces
    else if (c == 0x09) {
        cursor_x = (cursor_x + TAB_WIDTH) & ~(TAB_WIDTH - 1);
    }
    // Carriage return
    else if (c == '\r') {
        cursor_x = 0;
    }
    // Newline
    else if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    }
    // Any other character
    else if (c >= ' ') {
        // We can find the position in the text memory
        // With -> pos = textmemoryptr + (y * width + x)
        pos = textmemoryptr + (cursor_y * 80 + cursor_x);
        *pos = c | attr;
        cursor_x++;
    }

    // Check if we need to scroll
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

// Print a full string (calling the out_ch function many times)
void out(char *text) {
    int i;
    for (i = 0; i < calclen(text); i++) {
        out_ch(text[i]);
    }
}

// Set the text color
void set_color(enum Color c) { color = c; }

// Finally init the VGA. Simple initialization commands. Setting the
// textmemoryptr and clearing the screen to start fresh
void init_vga() {
    textmemoryptr = (unsigned short *)0xB8000;
    clear();
}

// Miscelaneous functions

void reset_color() { color = 0x0F; }

void kernel_log(char *msg) {
    set_color(LIGHT_GRAY);
    out("kernel: ");
    out(msg);
    reset_color();
}

void error(char *msg) {
    set_color(LIGHT_RED);
    out("error: ");
    out(msg);
    reset_color();
}

void panic(char *msg) {
    set_color(LIGHT_RED);
    out("avery panicked: ");
    out(msg);
    while (1) {
        // Halt the system
        __asm__ __volatile__("hlt");
    }
}
