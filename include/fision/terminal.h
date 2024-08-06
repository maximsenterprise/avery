/*
 fision.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Functions that correspond to the kernel terminal
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef TERMINAL_H
#define TERMINAL_H

#include "interrupts/isr.h"
void process_input(const char *input);

// Capture command
void capture(const char **input);
void capture_scancodes(struct regs *r);
extern unsigned int current_scancode;

#endif  // TERMINAL_H
