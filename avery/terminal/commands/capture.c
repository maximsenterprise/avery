/*
 capture.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Capture command for Fision
 Copyright (c) 2024 Maxims Enterprise
*/

#include "drivers/keyboard.h"
#include "fision/terminal.h"
#include "input.h"
#include "interrupts/irq.h"
#include "output.h"
#include "system/io.h"
#include "utils.h"

unsigned int current_scancode = 0;

void capture(const char **args) {
    if (args[1] == 0) {
        out("Capture integrated command for Fision\n");
        out("Function: Capture the keystrokes of the user in different ways\n");
        out("Arguments:\n");
        out("--------------------\n");
        out("-s: Capture the scancodes for the keystrokes\n");
        out("-k: Capture the ASCII values for the keystrokes\n");
        out("-b: Capture the binary values for the keystrokes\n");
        out("-h: Display this help message");
        return;
    } else if (strcompare(args[1], "-h")) {
        out("Capture integrated command for Fision\n");
        out("Function: Capture the keystrokes of the user in different ways\n");
        out("Arguments:\n");
        out("--------------------\n");
        out("-s: Capture the scancodes for the keystrokes\n");
        out("-k: Capture the ASCII values for the keystrokes\n");
        out("-b: Capture the binary values for the keystrokes");
        out("-h: Display this help message");
        return;
    } else if (strcompare(args[1], "-s")) {
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
    } else if (strcompare(args[1], "-k")) {
        // Get the ASCII value of the different characters
        while (1) {
            char *val = input("");
            if (strcompare(val, "\n") || strcompare(val, "")) {
                break;
            }
        }
    } else if (strcompare(args[1], "-b")) {
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
                    char bin_str[32];
                    hex_to_bin(current_scancode, bin_str);
                    out(bin_str);
                    out("\n");
                    last = current_scancode;
                }
            }
        }
        irq_uninstall_handler(1);
    } else if (strcompare(args[1], "-v")) {
        out("Capture is integrated with Fision\n");
        out("No version provided\n");
    } else {
        out("Invalid argument for capture\n");
        out("Use 'capture -h' for help");
    }
}

void capture_scancodes(struct regs *r) {
    unsigned int scancode = inb(0x60);
    current_scancode = scancode;
}
