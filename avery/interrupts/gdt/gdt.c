/*
 gdt.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Global Descriptor Table functions
 Copyright (c) 2024 Maxims Enterprise
*/

#include "interrupts/gdt.h"

// Define the variables declared on GDT.h
struct gdt_entry gdt[3];
struct gdt_pointer gp;

// Create a function to set the value of a GDT entry
void gdt_set_gate(int num, unsigned long base, unsigned long limit,
                  unsigned char access, unsigned char gran) {
    // Set the base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Set the limits
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Set the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Create a function to initialize the GDT
void init_gdt() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);                 // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment

    flush();
}
