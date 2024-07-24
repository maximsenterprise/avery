/*
 idt.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: IDT functions and structures
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef IDT_H
#define IDT_H

struct idt_entry {
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern void idt_set_gate(unsigned char num, unsigned long base,
                         unsigned short sel, unsigned char flags);

extern void init_idt();

extern struct idt_entry idt[256];
extern struct idt_ptr idtp;

extern void idt_load();

#endif  // IDT_H
