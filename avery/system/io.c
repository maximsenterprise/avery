/*
 io.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Functions that provide an abstraction to classic Assembly i/o
 functions Copyright (c) 2024 Maxims Enterprise
*/

#include "system/io.h"

unsigned char inb(unsigned short port) {
    unsigned char data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "dN"(port));
    return data;
}

void outb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %1, %0" : : "dN"(port), "a"(data));
}

void outw(coreuint16_t port, coreuint16_t value) {
    __asm__ __volatile__("outw %0, %1" : : "a"(value), "Nd"(port));
}

coreuint16_t inw(coreuint16_t port) {
    coreuint16_t data;
    __asm__ __volatile__("inw %1, %0" : "=a"(data) : "dN"(port));
    return data;
}
