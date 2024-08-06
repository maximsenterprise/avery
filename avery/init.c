/*
 init.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Entry point for Avery
 Copyright (c) 2024 Maxims Enterprise
*/

#include "fision/terminal.h"
#include "fs.h"
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
    hdd_setup();

    char file_list[1024];
    unsigned long file_list_size = sizeof(file_list);
    os_fat16_get_file_list(file_list, file_list_size);

    while (1) {
        char* in = input("> ");
        process_input(in);
        out("\n");
    }
}
