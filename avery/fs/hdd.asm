	; hdd.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: HDD functions for Avery. Based on BareMetalOS's implementation
	; Copyright (c) 2024 Maxims Enterprise

	; =============================================================================
	; BareMetal -- a 64-bit OS written in Assembly for x86-64 systems
	; Copyright (C) 2008-2010 Return Infinity -- see LICENSE.TXT

	; Hard Drive Functions
	; =============================================================================

	align 16
	db    'DEBUG: HDD      '
	align 16

	section .data
	message db 'a'; Character to print
	len     equ $ - message

	section .text

	global writesectors
	global readsectors

	extern out
	extern print_string

	; NOTE: These functions use LBA28. Maximum visible drive size is 128GiB
	; LBA48 would be needed to access sectors over 128GiB (up to 128PiB)
	; These functions are hard coded to access the Primary Master HDD

	; -----------------------------------------------------------------------------
	; readsectors -- Read sectors on the hard drive
	; IN: RAX = starting sector to read
	; RCX = number of sectors to read (1 - 256)
	; RDI = memory location to store sectors
	; OUT: RAX = RAX + number of sectors that were read
	; RCX = number of sectors that were read (0 on error)
	; RDI = RDI + (number of sectors * 512)
	; All other registers preserved

readsectors:
	;    Save registers
	push edx
	push ecx
	push eax
	push esi

	;   Setup LBA Address
	mov dx, 0x01F3; LBA Low Port
	mov al, [eax]; Load LBA Low byte
	out dx, al

	mov dx, 0x01F4; LBA Mid Port
	shr eax, 8
	mov al, al; Load LBA Mid byte
	out dx, al

	mov dx, 0x01F5; LBA High Port
	shr eax, 8
	mov al, al; Load LBA High byte
	out dx, al

	mov dx, 0x01F6; Device Port
	shr eax, 8
	and al, 0x0F; Clear bits 4-7 just to be safe
	or  al, 0x40; Turn bit 6 on for LBA mode
	out dx, al

	mov dx, 0x01F7; Command Port
	mov al, 0x20; Read sector(s)
	out dx, al

readsectors_wait:
	in   al, 0x01F7; Status Port
	test al, 0x08; Check if the sector buffer is ready
	jz   readsectors_wait; Wait if not ready

	;   Read data into memory
	;   Assuming ECX contains the number of words to read
	;   Ensure ECX and EDI are correctly set before this
	mov dx, 0x01F0; Data port
	rep insw; Read data to the address in EDI

	;   Restore registers
	pop edx
	pop ecx
	pop eax
	pop esi
	ret

readsectors_fail:
	;   Restore registers on failure
	pop esi
	pop eax
	pop ecx
	pop edx
	xor ecx, ecx; Set ECX to 0 since nothing was read
	ret
	; -----------------------------------------------------------------------------

	; -----------------------------------------------------------------------------
	; writesectors -- Write sectors on the hard drive
	; IN: RAX = starting sector to write
	; RCX = number of sectors to write (1 - 256)
	; RSI = memory location of sectors
	; OUT: RAX = RAX + number of sectors that were written
	; RCX = number of sectors that were written (0 on error)
	; RSI = RSI + (number of sectors * 512)
	; All other registers preserved

writesectors:
	push edx
	push ecx
	push eax

	mov ebx, ecx; Save ECX for use in the write loop
	cmp ebx, 256
	jg  writesectors_fail; Over 256? Fail!
	jne writesectors_skip; Not 256? No need to modify CL
	xor ebx, ebx; 0 translates to 256

writesectors_skip:
	push eax; Save EAX since we are about to overwrite it
	mov  dx, 0x01F2; 0x01F2 - Sector count Port 7:0
	mov  al, bl; Write BL sectors
	out  dx, al
	pop  eax; Restore EAX which is our sector number

	mov dx, 0x01F3; 0x01F3 - LBA Low Port 7:0
	mov al, al; Write LBA Low byte
	out dx, al

	mov dx, 0x01F4; 0x01F4 - LBA Mid Port 15:8
	shr eax, 8
	mov al, al; Write LBA Mid byte
	out dx, al

	mov dx, 0x01F5; 0x01F5 - LBA High Port 23:16
	shr eax, 8
	mov al, al; Write LBA High byte
	out dx, al

	mov dx, 0x01F6; 0x01F6 - Device Port. Bit 6 set for LBA mode, Bit 4 for device (0 = master, 1 = slave), Bits 3-0 for LBA "Extra High" (27:24)
	shr eax, 8
	and al, 0x0F; Clear bits 4-7 just to be safe
	or  al, 0x40; Turn bit 6 on since we want to use LBA addressing, leave device at 0 (master)
	out dx, al

	mov dx, 0x01F7; 0x01F7 - Command Port
	mov al, 0x30; Write sector(s). 0x34 if LBA48
	out dx, al

writesectors_wait:
	in   al, dx
	mov  ah, al; Move status byte to AH
	test ah, 0x80; Check BSY flag (Bit 7)
	jnz  writesectors_wait; Don't continue until the drive is ready.
	test ah, 0x08; Check DRQ flag (Bit 3)
	jz   writesectors_wait

	pop ecx
	shl ecx, 8; Multiply ECX by 256 to get the amount of words that will be written
	mov dx, 0x01F0; Data port - data comes in and out of here.
	rep outsw; Write data from the address starting at RSI

	mov dx, 0x01F7; Set DX back to the Command Port (0x01F7)

writesectors_wait_again:
	in   al, dx
	mov  bl, al; Move status byte to BL
	test bl, 0x80; Check BSY flag (Bit 7)
	jnz  writesectors_wait_again

	pop eax
	add eax, ecx
	pop ecx
	pop edx
	ret

writesectors_fail:
	pop ecx
	pop eax
	pop ecx
	pop edx
	xor ecx, ecx; Set ECX to 0 since nothing was written
	ret

