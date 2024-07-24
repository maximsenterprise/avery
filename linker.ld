 /**/
 /* linker.ld */
 /* As part of the Avery project */
 /* Created by Maxims Enterprise in 2024 */
 /* -------------------------------------------------- */
 /* Description: Script for the linker of the kernel */
 /* Copyright (c) 2024 Maxims Enterprise */
 /**/

OUTPUT_FORMAT(elf32-i386)
        ENTRY(start)
        SECTIONS
        {
            . = 0x100000;
            .text : { *(.text) }
            .data : { *(.data) }
            .bss  : { *(.bss) }
        }
