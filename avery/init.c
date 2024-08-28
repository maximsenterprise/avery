/*
 init.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Entry point for Avery
 Copyright (c) 2024 Maxims Enterprise
*/

#include "fision/terminal.h"
#include "input.h"
#include "interrupts/gdt.h"
#include "interrupts/idt.h"
#include "interrupts/irq.h"
#include "interrupts/isr.h"
#include "output.h"
#include "utils.h"
#include "vga.h"

void init() {
    // Initialize other systems
    init_gdt();
    init_idt();
    init_isrs();
    init_irqs();

    init_vga();

    // Main kernel loop
    //--------------------------
    clear();
    out("Avery Kernel\n");
    out("Version Alpha 0.0.1\n");
    out("Created by Maxims Enterprise\n\n");

    while (1) {
        char* in = input("> ");
        process_input(in);
        out("\n");
    }
}
