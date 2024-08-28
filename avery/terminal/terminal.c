/*
 terminal.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: The basic MScript Terminal
 Copyright (c) 2024 Maxims Enterprise
*/

#include "fision/terminal.h"

#include "drivers/acpi.h"
#include "fision/medit.h"
#include "output.h"
#include "utils.h"

void process_input(const char* input) {
    int delim = count_delim(input, ' ');
    char** parts = split(input, ' ', &delim);
    if (strcompare(input, "cls") || strcompare(input, "clear")) {
        clear();
    } else if (strcompare(input, "avery")) {
        out("Avery Kernel\n");
        out("Version Alpha 0.0.1\n");
        out("Created by Maxims Enterprise\n");
    } else if (strcompare(input, "fision")) {
        out("Fision\n");
        out("Version Alpha 0.0.1\n");
        out("Created by Maxims Enterprise\n");
    } else if (strcompare(input, "medit")) {
        medit();
    } else if (strcompare(parts[0], "capture")) {
        capture((const char**)parts);
    } else if (strcompare(parts[0], "shutdown")) {
#ifdef DEBUG
        debug_shutdown();
#else
        poweroff();
#endif
    } else {
        set_color(LIGHT_RED);
        out("Command not found: ");
        out((char*)input);
        set_color(WHITE);
    }
}
