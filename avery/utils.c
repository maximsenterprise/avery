/*
 utils.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Define utility functions for the Avery Kernel
 Copyright (c) 2024 Maxims Enterprise
*/

#include "utils.h"

#define MAX_TOKENS 100
#define MAX_TOKENS_LENGTH 100

// Copy memory from source to destination
unsigned char *memory_copy(unsigned char *dest, const unsigned char *src,
                           int count) {
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
    return dest;
}

// Set memory to a value
unsigned char *memory_set(unsigned char *dest, unsigned char val, int count) {
    for (int i = 0; i < count; i++) {
        dest[i] = val;
    }
    return dest;
}

// Set memory to a value
// The same as above, but for 16-bit values
unsigned short *memory_setw(unsigned short *dest, unsigned short value,
                            int count) {
    for (int i = 0; i < count; ++i) {
        dest[i] = value;
    }
    return dest;
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
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return *str1 == *str2;
}

// Function to copy strings from one variable to another
char *strcopy(char *dest, const char *src, int count) {
    int i;
    for (i = 0; i < count; i++) {
        dest[i] = src[i];
    }
    for (; i < count; i++) {
        dest[i] = '\0';
    }
    return dest;
}

// Function to find a character in a string
char *strchar(const char *str, char c) {
    while (*str) {
        if (*str == c) {
            return (char *)str;
        }
        str++;
    }
    return 0;
}

// Enable interrupts
void enable_interrupts() { __asm__ __volatile__("sti"); }

// Disable interrupts
void disable_interrupts() { __asm__ __volatile__("cli"); }

// Check if a character is a whitespace
int is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Function to count the delimiters in a string
int count_delim(const char *str, char delim) {
    int count = 0;
    while (*str) {
        if (*str == delim) {
            count++;
        }
        str++;
    }
    return count;
}

// Split a string by a delimiter
char **split(const char *str, char delim, int *num_tokens) {
    static char tokens[MAX_TOKENS][MAX_TOKENS_LENGTH];
    static char *token_ptrs[MAX_TOKENS];

    *num_tokens = count_delim(str, delim) + 1;
    if (*num_tokens > MAX_TOKENS) {
        *num_tokens = MAX_TOKENS;
    }

    int index = 0;
    const char *start = str;
    const char *end = strchar(str, delim);

    while (end != 0 && index < 100) {
        int length = end - start;
        if (length >= MAX_TOKENS_LENGTH) {
            length = MAX_TOKENS_LENGTH - 1;
        }
        strcopy(tokens[index], start, length);
        tokens[index][length] = '\0';
        token_ptrs[index] = tokens[index];
        index++;
        start = end + 1;
        end = strchar(start, delim);
    }

    if (index < 100) {
        strcopy(tokens[index], start, MAX_TOKENS_LENGTH - 1);
        tokens[index][MAX_TOKENS_LENGTH - 1] = '\0';
        token_ptrs[index] = tokens[index];
    }

    return token_ptrs;
}
