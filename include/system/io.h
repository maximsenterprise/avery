/*
 io.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: [h]: Definition for low level i/o methods
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef IO_H
#define IO_H

#include "utils.h"

extern unsigned char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char data);
extern void outw(coreuint16_t port, coreuint16_t value);
extern coreuint16_t inw(coreuint16_t port);

#endif  // IO_H
