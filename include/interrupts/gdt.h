/*
 gdt.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: GDT functions and structures
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef GDT_H
#define GDT_H

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern struct gdt_entry gdt[3];
extern struct gdt_ptr gp;

extern void gdt_flush();
extern void gdt_set_gate(int num, unsigned long base, unsigned long limit,
                         unsigned char access, unsigned char gran);
extern void init_gdt();

#endif  // GDT_H
