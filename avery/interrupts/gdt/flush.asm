	; flush.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: Flush functions in assembly
	; Copyright (c) 2024 Maxims Enterprise

	section .text
	global  flush
	extern  gp

flush:
	lgdt [gp]
	mov  ax, 0x10
	mov  ds, ax
	mov  es, ax
	mov  fs, ax
	mov  gs, ax
	mov  ss, ax
	jmp  0x08:stage2

stage2:
	ret
