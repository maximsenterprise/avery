/*
 vga.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: [h]: VGA functions and methods
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef VGA_H
#define VGA_H

/*
 * From all VGA functions available to do. Here are declared:
 * - scroll()
 * - move_cursor()
 * - init_vga()
 *   *********
 *   This function should be called only in the init() function
 * [var]: textmemoryptr
 * [var]: cursor_x, cursor_y
 * [var]: color
 * [const]: TAB_WIDTH
 */

#define TAB_WIDTH 8

extern void scroll();
extern void move_cursor();
extern void init_vga();

extern unsigned short *textmemoryptr;
extern unsigned int cursor_x, cursor_y;
extern unsigned int color;

#endif  // VGA_H
