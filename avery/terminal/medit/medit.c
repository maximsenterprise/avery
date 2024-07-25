/*
 medit.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: The main MEdit program file
 Copyright (c) 2024 Maxims Enterprise
*/

#include "mscript/medit.h"

#include "drivers/keyboard.h"
#include "input.h"
#include "output.h"
#include "utils.h"
#include "vga.h"

void medit() {
    clear();
    set_color(LIGHT_GRAY);
    for (int i = 0; i < 25; i++) {
        out("$\n");
    }
    set_color(WHITE);
    set_cursor_pos(2, 0);
    enable_keyboard();
    int current_x = 2;
    int current_y = 0;
    int chars = 0;

    char buffer[1024];
    int line_lengths[40];

    while (1) {
        char c = get_ch();
        if (c == '\n') {
            current_x = 2;
            current_y++;
            set_cursor_pos(current_x, current_y);
            chars++;
            buffer[chars] = '\n';
        } else if (c == 0x08) {
            if (current_x > 2) {
                out_ch(c);
                current_x--;
                set_cursor_pos(current_x, current_y);
                chars--;
                buffer[chars] = 0;
                line_lengths[current_y]--;
            } else {
                current_x = line_lengths[current_y - 1] + 1;
                current_y--;
                set_cursor_pos(current_x, current_y);
            }
        } else if (c >= ' ') {
            out_ch(c);
            current_x++;
            chars++;
            buffer[chars] = c;
            line_lengths[current_y]++;
        }
    }
    disable_keyboard();
}
