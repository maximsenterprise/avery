	
	; idt_load.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: The simple idtload function
	; Copyright (c) 2024 Maxims Enterprise
	

	global idt_load
	extern idtp

idt_load:
	lidt [idtp]
	ret
