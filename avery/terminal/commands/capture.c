/*
 capture.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Capture command for Fision
 Copyright (c) 2024 Maxims Enterprise
*/

#include "fision/terminal.h"
#include "interrupts/irq.h"
#include "output.h"
#include "system/io.h"
#include "utils.h"

unsigned int current_scancode = 0;

void capture(const char **input) {
    if (input[1] == 0) {
        out("Capture integrated command for Fision\n");
        out("Function: Capture the keystrokes of the user in different ways\n");
        out("Arguments:\n");
        out("--------------------\n");
        out("-s: Capture the scancodes for the keystrokes\n");
        out("-k: Capture the ASCII values for the keystrokes\n");
        out("-b: Capture the binary values for the keystrokes\n");
        out("-h: Display this help message");
        return;
    } else if (strcompare(input[1], "-h")) {
        out("Capture integrated command for Fision\n");
        out("Function: Capture the keystrokes of the user in different ways\n");
        out("Arguments:\n");
        out("--------------------\n");
        out("-s: Capture the scancodes for the keystrokes\n");
        out("-k: Capture the ASCII values for the keystrokes\n");
        out("-b: Capture the binary values for the keystrokes\n");
        out("-h: Display this help message");
        return;
    } else if (strcompare(input[1], "-s")) {
        // Get the scancodes of the different inputs
        irq_install_handler(1, capture_scancodes);
        unsigned int last = 0;
        while (1) {
            if (current_scancode == 0x1) {
                current_scancode = 0;
                break;
            } else if (current_scancode & 0x80) {
                // Key release
                last = 0;
            } else if (current_scancode != 0 && current_scancode != 0x9d) {
                if (current_scancode != last) {
                    out_hex(current_scancode);
                    out("\n");
                    last = current_scancode;
                }
            }
        }
        irq_uninstall_handler(1);
    }
}

void capture_scancodes(struct regs *r) {
    unsigned int scancode = inb(0x60);
    current_scancode = scancode;
}
