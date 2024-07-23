/*
 gdt.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: [h]: GDT parameters, functions and structures
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef GDT_H
#define GDT_H

// Basic Entry structure
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

// Pointer structure
struct gdt_pointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern struct gdt_entry gdt[3];
extern struct gdt_pointer gp;

// Function to flush the GDT
extern void flush();
extern void init_gdt();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit,
                         unsigned char access, unsigned char gran);

#endif  // GDT_H
