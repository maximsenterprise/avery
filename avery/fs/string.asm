	; string.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: String functions for Avery. Based on BareMetalOS's implementation
	; Copyright (c) 2024 Maxims Enterprise

	; =============================================================================
	; BareMetal -- a 64-bit OS written in Assembly for x86-64 systems
	; Copyright (C) 2008-2010 Return Infinity -- see LICENSE.TXT

	; String Functions
	; =============================================================================

	global os_string_length
	global os_string_uppercase
	global print_string

	; -----------------------------------------------------------------------------
	; os_string_length -- Return length of a string
	; IN: RSI = string location
	; OUT: RCX = length (not including the NULL terminator)
	; All other registers preserved

os_string_length:
	push edi
	push eax

	xor   ecx, ecx
	xor   eax, eax
	mov   edi, esi
	not   ecx
	cld
	repne scasb; compare byte at RDI to value in AL
	not   ecx
	dec   ecx

	pop eax
	pop edi
	ret

	; -----------------------------------------------------------------------------
	; os_string_uppercase -- Convert zero-terminated string to uppercase
	; IN: RSI = string location
	; OUT: All registers preserved

os_string_uppercase:
	push esi

os_string_uppercase_more:
	cmp byte [esi], 0x00; Zero-termination of string?
	je  os_string_uppercase_done; If so, quit
	cmp byte [esi], 97; In the uppercase A to Z range?
	jl  os_string_uppercase_noatoz
	cmp byte [esi], 122
	jg  os_string_uppercase_noatoz
	sub byte [esi], 0x20; If so, convert input char to lowercase
	inc esi
	jmp os_string_uppercase_more

os_string_uppercase_noatoz:
	inc esi
	jmp os_string_uppercase_more

os_string_uppercase_done:
	pop esi
	ret

print_string:
	;    Guardar el registro que usaremos
	push ebp
	mov  ebp, esp

	;   Configurar los argumentos para la syscall write
	mov ebx, 1; File descriptor 1 (stdout)
	mov edx, [esi-4]; Longitud del mensaje, que está en la dirección [esi-4]
	mov eax, 4; syscall número para sys_write

	;   Realizar la llamada al sistema
	int 0x80; Interrupción del sistema

	;   Restaurar el registro y volver
	mov esp, ebp
	pop ebp
	ret
