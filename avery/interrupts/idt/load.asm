	; load.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: Load function for the IDT
	; Copyright (c) 2024 Maxims Enterprise

	section .text
	global  load_idt
	extern  idtptr

load_idt:
	lidt [idtptr]
	ret
