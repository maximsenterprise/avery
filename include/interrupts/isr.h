/*
 isr.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: The implementation of the Interrupt Service Routines
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef ISR_H
#define ISR_H

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void init_isrs();
static unsigned char *exception_messages[] = {
    "Division by Zero occured (x0)",
    "Debug Exception occured (x1)",
    "Non Maskable Interrupt occured (x2)",
    "Breakpoint Exception occured (x3)",
    "Overflow Exception occured (x4)",
    "Bound Range Exceeded Exception occured (x5)",
    "Invalid Opcode Exception occured (x6)",
    "Device Not Available Exception occured (x7)",
    "Double Fault Exception occured (x8)",
    "Coprocessor Segment Overrun Exception occured (x9)",
    "Invalid TSS Exception occured (x10)",
    "Segment Not Present Exception occured (x11)",
    "Stack Fault Exception occured (x12)",
    "General Protection Fault Exception occured (x13)",
    "Page Fault Exception occured (x14)",
    "Unknown Interrupt Exception occured (x15)",
    "x87 FPU Floating Point Error Exception occured (x16)",
    "Alignment Check Exception occured (x17)",
    "Machine Check Exception occured (x18)",
    "SIMD Floating Point Exception occured (x19)",
    "Virtualization Exception occured (x20)",
    "Control Protection Exception occured (x21)",
    "Reserved (x22)",
    "Reserved (x23)",
    "Reserved (x24)",
    "Reserved (x25)",
    "Reserved (x26)",
    "Reserved (x27)",
    "Reserved (x28)",
    "Reserved (x29)",
    "Reserved (x30)",
    "Reserved (x31)"};

static struct regs {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
};

extern void fault_handler(struct regs *r);

#endif  // ISR_H
