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

extern unsigned char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char data);

#endif  // IO_H
