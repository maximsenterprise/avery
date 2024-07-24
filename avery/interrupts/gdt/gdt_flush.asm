	
	; gdt_flush.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: The GDT flush function
	; Copyright (c) 2024 Maxims Enterprise
	

	global gdt_flush
	extern gp

gdt_flush:
	lgdt [gp]
	mov  ax, 0x10
	mov  ds, ax
	mov  es, ax
	mov  fs, ax
	mov  gs, ax
	mov  ss, ax
	jmp  0x08:flush2

flush2:
	ret
