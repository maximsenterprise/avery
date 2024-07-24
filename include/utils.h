/*
 utils.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: [h]: Utility functions for the kernel -> C
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef UTILS_H
#define UTILS_H

unsigned char *memory_copy(unsigned char *dest, const unsigned char *src,
                           int nbytes);
unsigned char *memory_set(unsigned char *dest, unsigned char val, int len);
unsigned short *memory_setw(unsigned short *dest, unsigned short val, int len);
int calclen(const char *str);
int strcompare(const char *str1, const char *str2);
void enable_interrupts();
void disable_interrupts();

#endif  // UTILS_H
