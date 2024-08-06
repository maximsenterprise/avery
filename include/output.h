/*
 output.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: [h]: Output functions and utils
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef OUTPUT_H
#define OUTPUT_H

/*
 * From all VGA functions available to do. Here are declared:
 * - out(char *text)
 * - set_color(enum Color color)
 * - reset_color()
 * - out_ch(unsigned char c)
 * - kernel_log(char *msg) (To differentiate from 'log')
 * - error(char *msg)
 * - panic(char *msg)
 *   ^^^^^^^^^^^^^^^^^^^^^^^^^
 *   This 'halts' the system when called
 * - clear()
 * - out_hex(unsigned char c)
 */

enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
};

extern void out(char *text);
extern void set_color(enum Color color);
extern void out_ch(unsigned char c);
extern void kernel_log(char *msg);
extern void error(char *msg);
extern void panic(char *msg);
extern void reset_color();
extern void clear();
extern void out_hex(unsigned int n);

#endif  // OUTPUT_H
