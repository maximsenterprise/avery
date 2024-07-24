/*
 terminal.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: The basic MScript Terminal
 Copyright (c) 2024 Maxims Enterprise
*/

#include "mscript/terminal.h"

#include "output.h"
#include "utils.h"

void process_input(const char* input) {
    if (strcompare(input, "cls") || strcompare(input, "clear")) {
        clear();
    } else if (strcompare(input, "avery")) {
        out("Avery Kernel\n");
        out("Version Alpha 0.0.1\n");
        out("Created by Maxims Enterprise\n");
    } else if (strcompare(input, "mscript")) {
        out("MScript\n");
        out("Version Alpha 0.0.1\n");
        out("Created by Maxims Enterprise\n");
    } else {
        out("Command not found: ");
        out((char*)input);
    }
}
