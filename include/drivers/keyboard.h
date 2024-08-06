/*
 keyboard.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Keyboard utilities and functions
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupts/isr.h"

// Layouts

// USA
extern unsigned char kbdus[128];
extern unsigned char kbdus_shift[128];

// ESP
extern unsigned char kbdes[128];
extern unsigned char kbdes_shift[128];
extern unsigned char kbdes_opt[128];

enum KEYBOARD_LAYOUT { USA, ESP };

extern enum KEYBOARD_LAYOUT current_layout;

extern unsigned char* current;
extern unsigned char* current_shift;
extern unsigned char* current_opt;

extern char current_char;
extern char get_ch();

extern void keyboard_handler(struct regs* r);
extern void enable_keyboard();
extern void disable_keyboard();
extern void set_layout(enum KEYBOARD_LAYOUT layout);
extern int shift;
extern int opt;

#endif  // KEYBOARD_H
