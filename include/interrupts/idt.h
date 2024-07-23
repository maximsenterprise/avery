/*
 idt.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Definitions of functions for the IDT
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef IDT_H
#define IDT_H

// Basic Entry structure
struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

// Pointer structure
struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// External functions and variables
extern struct idt_entry idt[256];
extern struct idt_ptr idtptr;

extern void idt_set_gate(unsigned char num, unsigned long base,
                         unsigned short sel, unsigned char flags);
// Defined in load.asm
extern void load_idt();
extern void init_idt();

#endif  // IDT_H
