	; boot.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: Bootloader for the Avery Kernel
	; Copyright (c) 2024 Maxims Enterprise

	BITS    32
	section .text
	;       Multiboot header
	align   4
	dd      0x1BADB002
	dd      0x00
	dd      - (0x1BADB002 + 0x00)

	global _start
	extern init

_start:
	cli
	call init
	hlt

