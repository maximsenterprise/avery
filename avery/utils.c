/*
 utils.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Define utility functions for the Avery Kernel
 Copyright (c) 2024 Maxims Enterprise
*/

#include "utils.h"

// Copy memory from source to destination
unsigned char *memory_copy(unsigned char *dest, unsigned char *source,
                           int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
    return dest;
}

// Set memory to a value
unsigned char *memory_set(unsigned char *dest, unsigned char val, int len) {
    unsigned char *temp = (unsigned char *)dest;
    for (; len != 0; len--) *temp++ = val;
    return dest;
}

// Set memory to a value
// The same as above, but for 16-bit values
unsigned short *memory_setw(unsigned char *dest, unsigned short val, int len) {
    unsigned short *temp = (unsigned short *)dest;
    for (; len != 0; len--) *temp++ = val;
    return (unsigned short *)dest;
}

// Calculate the length of a string
int calclen(const char *str) {
    int len = 0;
    while (*str != 0) {
        len++;
        str++;
    }
    return len;
}

// Compare two strings
// To enforce string equality in C, beacuse there is not suitable '==' operator
// for two strings
int strcompare(const char *str1, const char *str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return 1;
}
