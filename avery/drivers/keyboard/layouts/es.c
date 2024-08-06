/*
 es.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Keyboard layout for Spanish Keyboards
 Copyright (c) 2024 Maxims Enterprise
*/

#include "drivers/keyboard.h"

unsigned char kbdes[128] = {
    0,    27,  '1',  '2', '3',  '4', '5', '6', '7',  '8', /* 9 */
    '9',  '0', '\'', 0,   '\b',                           /* Backspace */
    '\t',                                                 /* Tab */
    'q',  'w', 'e',  'r',                                 /* 19 */
    't',  'y', 'u',  'i', 'o',  'p', '`', '+', '\n',      /* Enter key */
    0,                                                    /* 29   - Control */
    'a',  's', 'd',  'f', 'g',  'h', 'j', 'k', 'l',  0,   /* 39 */
    '\'', 0,   0,                                         /* Left shift */
    '\\', 'z', 'x',  'c', 'v',  'b', 'n',                 /* 49 */
    'm',  ',', '.',  '-', 0,                              /* Right shift */
    '*',  0,                                              /* Alt */
    ' ',                                                  /* Space bar */
    0,                                                    /* Caps lock */
    0,                                            /* 59 - F1 key ... > */
    0,    0,   0,    0,   0,    0,   0,   0,   0, /* < ... F10 */
    0,                                            /* 69 - Num lock */
    0,                                            /* Scroll Lock */
    0,                                            /* Home key */
    0,                                            /* Up Arrow */
    0,                                            /* Page Up */
    '-',  0,                                      /* Left Arrow */
    0,    0,                                      /* Right Arrow */
    '+',  0,                                      /* 79 - End key */
    0,                                            /* Down Arrow */
    0,                                            /* Page Down */
    0,                                            /* Insert Key */
    0,                                            /* Delete Key */
    0,    0,   0,    0,                           /* F11 Key */
    0,                                            /* F12 Key */
    0, /* All other keys are undefined */
};

unsigned char kbdes_shift[128] = {
    0,    27,  '!', '"', 0,    '$', '%', '&', '/',  '(', /* 9 */
    ')',  '=', '?', 0,   '\b',                           /* Backspace */
    '\t',                                                /* Tab */
    'Q',  'W', 'E', 'R',                                 /* 19 */
    'T',  'Y', 'U', 'I', 'O',  'P', '^', '*', '\n',      /* Enter key */
    0,                                                   /* 29   - Control */
    'A',  'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L',  0,   /* 39 */
    0,    0,   0,                                        /* Left shift */
    '>',  'Z', 'X', 'C', 'V',  'B', 'N',                 /* 49 */
    'M',  ';', ':', '_', 0,                              /* Right shift */
    '*',  0,                                             /* Alt */
    ' ',                                                 /* Space bar */
    0,                                                   /* Caps lock */
    0,                                                   /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,    0,   0,   0,   0,         /* < ... F10 */
    0,                                                   /* 69 - Num lock */
    0,                                                   /* Scroll Lock */
    0,                                                   /* Home key */
    0,                                                   /* Up Arrow */
    0,                                                   /* Page Up */
    '-',  0,                                             /* Left Arrow */
    0,    0,                                             /* Right Arrow */
    '+',  0,                                             /* 79 - End key */
    0,                                                   /* Down Arrow */
    0,                                                   /* Page Down */
    0,                                                   /* Insert Key */
    0,                                                   /* Delete Key */
    0,    0,   0,   0,                                   /* F11 Key */
    0,                                                   /* F12 Key */
    0, /* All other keys are undefined */
};

unsigned char kbdes_opt[128] = {
    0,    27,  '|', '@', '#', 0, 0,   0,   0,    0, /* 9 */
    0,    0,   0,   0,   0,                         /* Backspace */
    '\t',                                           /* Tab */
    0,    0,   0,   0,                              /* 19 */
    0,    0,   0,   0,   0,   0, '[', ']', '\n',    /* Enter key */
    0,                                              /* 29   - Control */
    0,    0,   0,   0,   0,   0, 0,   0,   0,    0, /* 39 */
    '{',  '}', '}',                                 /* Left shift */
    '}',  0,   0,   0,   0,   0, 0,                 /* 49 */
    0,    0,   0,   0,   0,                         /* Right shift */
    '*',  0,                                        /* Alt */
    ' ',                                            /* Space bar */
    0,                                              /* Caps lock */
    0,                                              /* 59 - F1 key ... > */
    0,    0,   0,   0,   0,   0, 0,   0,   0,       /* < ... F10 */
    0,                                              /* 69 - Num lock */
    0,                                              /* Scroll Lock */
    0,                                              /* Home key */
    0,                                              /* Up Arrow */
    0,                                              /* Page Up */
    0,    0,                                        /* Left Arrow */
    0,    0,                                        /* Right Arrow */
    0,    0,                                        /* 79 - End key */
    0,                                              /* Down Arrow */
    0,                                              /* Page Down */
    0,                                              /* Insert Key */
    0,                                              /* Delete Key */
    0,    0,   0,   0,                              /* F11 Key */
    0,                                              /* F12 Key */
    0, /* All other keys are undefined */
};
