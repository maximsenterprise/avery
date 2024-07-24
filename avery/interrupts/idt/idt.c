/*
 idt.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: The Interrupt Descriptor Table implementation
 Copyright (c) 2024 Maxims Enterprise
*/

#include "interrupts/idt.h"

#include "output.h"
#include "utils.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

// Set the gate at index num in the IDT
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel,
                  unsigned char flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void init_idt() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    memory_set((unsigned char *)idt, 0, sizeof(struct idt_entry) * 256);

    idt_load();
}
