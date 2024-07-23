/*
 init.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: Entry point for Avery
 Copyright (c) 2024 Maxims Enterprise
*/

#include "output.h"
#include "vga.h"

void init() {
    // Initialize other systems
    init_vga();

    // Main kernel loop
    //--------------------------
    clear();
    out("Avery Kernel\n");
    out("Version Alpha 0.0.1\n");
    out("Created by Maxims Enterprise\n");
}
