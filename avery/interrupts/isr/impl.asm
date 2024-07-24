	
	; impl.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: Implmentation of the ISR routines
	; Copyright (c) 2024 Maxims Enterprise
	

	global isr0
	global isr1
	global isr2
	global isr3
	global isr4
	global isr5
	global isr6
	global isr7
	global isr8
	global isr9
	global isr10
	global isr11
	global isr12
	global isr13
	global isr14
	global isr15
	global isr16
	global isr17
	global isr18
	global isr19
	global isr20
	global isr21
	global isr22
	global isr23
	global isr24
	global isr25
	global isr26
	global isr27
	global isr28
	global isr29
	global isr30
	global isr31

	; 0: Divide By Zero Exception

isr0:
	cli
	push byte 0
	push byte 0
	jmp  isr_common_stub

	; 1: Debug Exception

isr1:
	cli
	push byte 0
	push byte 1
	jmp  isr_common_stub

	; 2: Non-Maskable Interrupt (NMI)

isr2:
	cli
	push byte 0
	push byte 2
	jmp  isr_common_stub

	; 3: Breakpoint Exception

isr3:
	cli
	push byte 0
	push byte 3
	jmp  isr_common_stub

	; 4: Overflow Exception

isr4:
	cli
	push byte 0
	push byte 4
	jmp  isr_common_stub

	; 5: Bound Range Exceeded Exception

isr5:
	cli
	push byte 0
	push byte 5
	jmp  isr_common_stub

	; 6: Invalid Opcode Exception

isr6:
	cli
	push byte 0
	push byte 6
	jmp  isr_common_stub

	; 7: Device Not Available Exception

isr7:
	cli
	push byte 0
	push byte 7
	jmp  isr_common_stub

	; 8: Double Fault Exception (With Error Code!)

isr8:
	cli
	push byte 8
	jmp  isr_common_stub

	; 9: Coprocessor Segment Overrun (Reserved)

isr9:
	cli
	push byte 0
	push byte 9
	jmp  isr_common_stub

	; 10: Invalid TSS Exception

isr10:
	cli
	push byte 0
	push byte 10
	jmp  isr_common_stub

	; 11: Segment Not Present Exception

isr11:
	cli
	push byte 0
	push byte 11
	jmp  isr_common_stub

	; 12: Stack-Segment Fault Exception

isr12:
	cli
	push byte 0
	push byte 12
	jmp  isr_common_stub

	; 13: General Protection Fault Exception

isr13:
	cli
	push byte 0
	push byte 13
	jmp  isr_common_stub

	; 14: Page Fault Exception

isr14:
	cli
	push byte 0
	push byte 14
	jmp  isr_common_stub

	; 15: Reserved

isr15:
	cli
	push byte 0
	push byte 15
	jmp  isr_common_stub

	; 16: x87 Floating-Point Exception (FPU Floating-Point Error)

isr16:
	cli
	push byte 0
	push byte 16
	jmp  isr_common_stub

	; 17: Alignment Check Exception (Machine Check Exception)

isr17:
	cli
	push byte 0
	push byte 17
	jmp  isr_common_stub

	; 18: Machine Check Exception

isr18:
	cli
	push byte 0
	push byte 18
	jmp  isr_common_stub

	; 19: SIMD Floating-Point Exception

isr19:
	cli
	push byte 0
	push byte 19
	jmp  isr_common_stub

	; 20-31: Reserved

isr20:
	cli
	push byte 0
	push byte 20
	jmp  isr_common_stub

isr21:
	cli
	push byte 0
	push byte 21
	jmp  isr_common_stub

isr22:
	cli
	push byte 0
	push byte 22
	jmp  isr_common_stub

isr23:
	cli
	push byte 0
	push byte 23
	jmp  isr_common_stub

isr24:
	cli
	push byte 0
	push byte 24
	jmp  isr_common_stub

isr25:
	cli
	push byte 0
	push byte 25
	jmp  isr_common_stub

isr26:
	cli
	push byte 0
	push byte 26
	jmp  isr_common_stub

isr27:
	cli
	push byte 0
	push byte 27
	jmp  isr_common_stub

isr28:
	cli
	push byte 0
	push byte 28
	jmp  isr_common_stub

isr29:
	cli
	push byte 0
	push byte 29
	jmp  isr_common_stub

isr30:
	cli
	push byte 0
	push byte 30
	jmp  isr_common_stub

isr31:
	cli
	push byte 0
	push byte 31
	jmp  isr_common_stub

extern fault_handler

isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov  ax, 0x10
	mov  ds, ax
	mov  es, ax
	mov  fs, ax
	mov  gs, ax
	mov  eax, esp
	push eax
	mov  eax, fault_handler
	call eax
	pop  eax
	pop  gs
	pop  fs
	pop  es
	pop  ds
	popa
	add  esp, 8
	iret
