/*
 idt.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Functions for the Interrupt Descriptor Table
 Copyright (c) 2024 Maxims Enterprise
*/

#include "interrupts/idt.h"

#include "utils.h"

struct idt_entry idt[256];
struct idt_ptr idtptr;

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
                  unsigned char flags) {
    // Set the base address
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    // Set the selector and flags
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void init_idt() {
    idtptr.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtptr.base = (unsigned int)&idt;

    // Clear the IDT
    memory_set((unsigned char*)&idt, 0, sizeof(struct idt_entry) * 256);

    // Load the IDT
    load_idt();
}
