/* linker.ld */
/* As part of the Avery project */
/* Created by Maxims Enterprise in 2024 */

OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)
SECTIONS {
	. = 0x00100000;
	.text : {
		*(.text)
	}
	.data : {
		*(.data)
	}
	.bss : {
		*(.bss)
	}
}


