	; fat16.asm
	; As part of the Avery project
	; Created by Maxims Enterprise in 2024
	; --------------------------------------------------
	; Description: Fat16 Support for Avery. Based on BareMetalOS's implementation
	; Copyright (c) 2024 Maxims Enterprise

	; =============================================================================
	; BareMetal -- a 32-bit OS written in Assembly for x86 systems
	; Copyright (C) 2008-2010 Return Infinity -- see LICENSE.TXT

	; FAT16 Functions
	; =============================================================================

	align 16
	db    'DEBUG: FAT16    '
	align 16

	;      Extern functions
	extern fat16_SectorsPerCluster
	extern fat16_DataStart
	extern fat16_PartitionOffset
	extern fat16_ReservedSectors
	extern fat16_RootStart
	extern readsectors
	extern writesectors
	extern secbuffer1
	extern hdbuffer1
	extern hdbuffer0
	extern os_string_length
	extern os_string_uppercase
	extern out

	global os_fat16_get_file_list

	; -----------------------------------------------------------------------------
	; os_fat16_read_cluster -- Read a cluster from the FAT16 partition
	; IN: EAX  = Cluster # to read
	; EDI = Memory location to store at least 32KB
	; OUT: EAX  = Next cluster in chain (0xFFFF if this was the last)
	; EDI = Points one byte after the last byte read

os_fat16_read_cluster:
	push ebx
	push ecx
	push edx
	push esi

	and eax, 0x0000FFFF; Clear the top 16 bits
	mov ebx, eax; Save the cluster number to be used later

	cmp ax, 2; If less than 2 then bail out...
	jl  near os_fat16_read_cluster_bailout; as clusters start at 2

	;    Calculate the LBA address --- startingsector = (cluster-2) * clustersize + data_start
	xor  ecx, ecx
	mov  cl, byte [fat16_SectorsPerCluster]
	push ecx; Save the number of sectors per cluster
	sub  eax, 2
	imul ecx; EAX now holds starting sector
	add  eax, dword [fat16_DataStart]; EAX now holds the sector where our cluster starts
	add  eax, [fat16_PartitionOffset]; Add the offset to the partition

	pop  ecx; Restore the number of sectors per cluster
	call readsectors; Read one cluster of sectors

	;     Calculate the next cluster
	push  edi
	mov   edi, secbuffer1; Read to this temporary buffer
	mov   esi, edi; Copy buffer address to ESI
	push  ebx; Save the original cluster value
	shr   ebx, 8; Divide the cluster value by 256. Keep no remainder
	movzx eax, word [fat16_ReservedSectors]; First sector of the first FAT
	add   eax, [fat16_PartitionOffset]; Add the offset to the partition
	add   eax, ebx; Add the sector offset
	mov   ecx, 1
	call  readsectors
	pop   ebx; Get our original cluster value back
	shl   ebx, 8; Quick multiply by 256 (EBX was the sector offset in the FAT)
	sub   eax, ebx; EAX is now pointed to the offset within the sector
	shl   eax, 1; Quickly multiply by 2 (since entries are 16-bit)
	add   esi, eax
	lodsw ; AX now holds the next cluster
	pop   edi

	jmp os_fat16_read_cluster_end

os_fat16_read_cluster_bailout:
	xor  eax, eax
	mov  eax, message
	call out

os_fat16_read_cluster_end:
	pop esi
	pop edx
	pop ecx
	pop ebx
	ret
	; -----------------------------------------------------------------------------

	; -----------------------------------------------------------------------------
	; os_fat16_write_cluster -- Write a cluster to the FAT16 partition
	; IN: EAX  = Cluster # to write to
	; ESI = Memory location of data to write
	; OUT: EAX  = Next cluster in the chain (set to 0xFFFF if this was the last cluster of the chain)
	; ESI = Points one byte after the last byte written

os_fat16_write_cluster:
	push edi
	push edx
	push ecx
	push ebx

	and eax, 0x0000FFFF; Clear the top 16 bits
	mov ebx, eax; Save the cluster number to be used later

	cmp ax, 2; If less than 2 then bail out...
	jl  near os_fat16_write_cluster_bailout; as clusters start at 2

	;    Calculate the LBA address --- startingsector = (cluster-2) * clustersize + data_start
	xor  ecx, ecx
	mov  cl, byte [fat16_SectorsPerCluster]
	push ecx; Save the number of sectors per cluster
	sub  eax, 2
	imul ecx; EAX now holds starting sector
	add  eax, dword [fat16_DataStart]; EAX now holds the sector where our cluster starts
	add  eax, [fat16_PartitionOffset]; Add the offset to the partition

	pop  ecx; Restore the number of sectors per cluster
	call writesectors

	;     Calculate the next cluster
	push  esi
	mov   edi, secbuffer1; Read to this temporary buffer
	mov   esi, edi; Copy buffer address to ESI
	push  ebx; Save the original cluster value
	shr   ebx, 8; Divide the cluster value by 256. Keep no remainder
	movzx eax, word [fat16_ReservedSectors]; First sector of the first FAT
	add   eax, [fat16_PartitionOffset]; Add the offset to the partition
	add   eax, ebx; Add the sector offset
	mov   ecx, 1
	call  readsectors
	pop   ebx; Get our original cluster value back
	shl   ebx, 8; Quick multiply by 256 (EBX was the sector offset in the FAT)
	sub   eax, ebx; EAX is now pointed to the offset within the sector
	shl   eax, 1; Quickly multiply by 2 (since entries are 16-bit)
	add   esi, eax
	lodsw ; AX now holds the next cluster
	pop   esi

	jmp os_fat16_write_cluster_done

os_fat16_write_cluster_bailout:
	xor eax, eax

os_fat16_write_cluster_done:
	pop ebx
	pop ecx
	pop edx
	pop edi
	ret
	; -----------------------------------------------------------------------------

	; -----------------------------------------------------------------------------
	; os_fat16_find_file -- Search for a file name and return the starting cluster
	; IN: ESI = Pointer to file name, must be in 'FILENAMEEXT' format
	; OUT: EAX  = Starting cluster
	; Carry set if not found. If carry is set then ignore value in EAX

os_fat16_find_file:
	push esi
	push edi
	push edx
	push ecx
	push ebx

	clc ; Clear carry
	xor eax, eax
	mov eax, [fat16_RootStart]; EAX points to the first sector of the root
	add eax, [fat16_PartitionOffset]; Add the offset to the partition
	mov edx, eax; Save the sector value

os_fat16_find_file_read_sector:
	mov  edi, hdbuffer1
	push edi
	mov  ecx, 1
	call readsectors
	pop  edi
	mov  ebx, 16; Each record is 32 bytes. 512 (bytes per sector) / 32 = 16

os_fat16_find_file_next_entry:
	cmp byte [edi], 0x00; end of records
	je  os_fat16_find_file_notfound

	mov  ecx, 11
	push esi
	repe cmpsb
	pop  esi
	mov  ax, [edi+15]; AX now holds the starting cluster # of the file we just looked at
	jz   os_fat16_find_file_done; The file was found. Note that EDI now is at dirent+11

	add edi, byte 0x20
	and edi, byte -0x20
	dec ebx
	cmp ebx, 0
	jne os_fat16_find_file_next_entry

	; At this point we have read through one sector of file names. We have not found the file we are looking for and have not reached the end of the table. Load the next sector.

	add edx, 1
	mov eax, edx
	jmp os_fat16_find_file_read_sector

os_fat16_find_file_notfound:
	stc ; Set carry
	xor eax, eax

os_fat16_find_file_done:
	cmp ax, 0x0000; BUG HERE
	jne wut; Carry is not being set properly in this function
	stc

wut:
	pop ebx
	pop ecx
	pop edx
	pop edi
	pop esi
	ret

	; -----------------------------------------------------------------------------
	; os_fat16_get_file_list -- Generate a list of files on disk
	; IN: EDI = location to store the file list
	; OUT: EDI = Points one byte after the last byte written

os_fat16_get_file_list:
	push ebx
	push ecx
	push edx
	push esi
	push edi

	mov eax, [fat16_RootStart]
	add eax, [fat16_PartitionOffset]; EAX points to the first sector of the root
	mov edx, eax; Save the sector value

os_fat16_get_file_list_read_sector:
	mov  edi, hdbuffer1
	push edi
	mov  ecx, 1

	call readsectors
	pop  edi
	mov  ebx, 16; Each record is 32 bytes. 512 / 32 = 16

os_fat16_get_file_list_next_entry:
	cmp byte [edi], 0x00; end of records
	je  os_fat16_get_file_list_done

	;   Check if the file entry is not empty (i.e., check for the file's name length)
	cmp byte [edi+0], 0x00
	je  os_fat16_get_file_list_skip_entry
	cmp byte [edi+0], 0xE5; Check for deleted file
	je  os_fat16_get_file_list_skip_entry

	;   Copy the file name to the output buffer
	mov ecx, 11
	rep movsb

	;     Add a null terminator after the file name
	mov   byte [edi+11], 0x00
	movzx eax, word [edi+15]; Get the starting cluster
	mov   [edi+12], eax; Store it in the file entry

	;   Move the pointer to the next file entry
	add edi, byte 0x20
	and edi, byte -0x20
	dec ebx
	cmp ebx, 0
	jne os_fat16_get_file_list_next_entry

	;   Read the next sector
	add edx, 1
	mov eax, edx
	jmp os_fat16_get_file_list_read_sector

os_fat16_get_file_list_skip_entry:
	add edi, byte 0x20
	and edi, byte -0x20
	dec ebx
	cmp ebx, 0
	jne os_fat16_get_file_list_next_entry

	;   Read the next sector if needed
	add edx, 1
	mov eax, edx
	jmp os_fat16_get_file_list_read_sector

os_fat16_get_file_list_done:
	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	ret
	; -----------------------------------------------------------------------------

	; -----------------------------------------------------------------------------
	; os_fat16_file_read -- Read a file from disk into memory
	; IN: RSI = Address of filename string
	; RDI = Memory location where file will be loaded to
	; OUT: Carry clear on success, set if file was not found or error occurred

os_fat16_file_read:
	push esi
	push edi
	push eax

	;    Convert the file name to FAT format
	push edi; Save the memory address
	mov  edi, os_fat16_file_read_string
	call os_fat16_filename_convert; Convert the filename to the proper FAT format
	xchg esi, edi
	pop  edi; Grab the memory address
	jc   os_fat16_file_read_done; If Carry is set then the filename could not be converted

	;    Check to see if the file exists
	call os_fat16_find_file; Function will return the starting cluster value in AX or carry set if not found
	jc   os_fat16_file_read_done; If Carry is clear then the file exists. AX is set to the starting cluster

os_fat16_file_read_read:
	call os_fat16_read_cluster; Store cluster in memory. AX is set to the next cluster
	cmp  ax, 0xFFFF; 0xFFFF is the FAT end of file marker
	jne  os_fat16_file_read_read; Are there more clusters? If so then read again.. if not fall through
	clc  ; Clear Carry

os_fat16_file_read_done:
	pop eax
	pop edi
	pop esi
	ret

os_fat16_file_read_string times 13 db 0

	; -----------------------------------------------------------------------------
	; os_fat16_file_write -- Write a file to the hard disk
	; IN: RSI = Address of data in memory
	; RDI = File name to write
	; RCX = number of bytes to write
	; OUT: Carry clear on success, set on failure

os_fat16_file_write:
	push esi
	push edi
	push ecx
	push eax

	mov [memory_address], esi; Save the memory address

	;    Convert the file name to FAT format
	mov  esi, edi; Move the file name address into ESI
	mov  edi, os_fat16_file_write_string
	call os_fat16_filename_convert; Convert the filename to the proper FAT format
	jc   os_fat16_file_write_done; Fail (Invalid file name)

	;    Check to see if a file already exists with the same name
	mov  esi, os_fat16_file_write_string
	call os_fat16_find_file; Returns the starting cluster in AX or carry set if it doesn't exist
	jc   os_fat16_file_write_create; Jump if the file doesn't exist (No need to delete it)
	jmp  os_fat16_file_write_done; call os_fat16_file_delete

	; At this point the file doesn't exist so create it.

os_fat16_file_write_create:
	call os_fat16_file_create
	jc   os_fat16_file_write_done; Fail (Couldn't create the file)
	call os_fat16_find_file; Call this to get the starting cluster
	jc   os_fat16_file_write_done; Fail (File was supposed to be created but wasn't)

	;   We are ready to start writing. First cluster is in AX
	mov esi, [memory_address]

os_fat16_file_write_write:
	call os_fat16_write_cluster
	cmp  ax, 0xFFFF
	jne  os_fat16_file_write_write
	clc

os_fat16_file_write_done:
	pop eax
	pop ecx
	pop edi
	pop esi
	ret

os_fat16_file_write_string times 13 db 0
memory_address   dd 0x00000000

	; -----------------------------------------------------------------------------
	; os_fat16_file_create -- Create a file on the hard disk
	; IN: ESI = Pointer to file name, must be in FAT 'FILENAMEEXT' format
	; ECX = File size
	; OUT: Carry clear on success, set on failure
	; Note: This function pre-allocates all clusters required for the size of the file

os_fat16_file_create:
	push esi
	push edi
	push edx
	push ecx
	push ebx
	push eax

	clc ; Clear the carry flag. It will be set if there is an error

	mov [filesize], ecx; Save file size for later
	mov [filename], esi

	; Check to see if a file already exists with the same name
	; call os_fat16_find_file
	; jc os_fat16_file_create_fail; Fail (File already exists)

	;   How many clusters will we need?
	mov eax, ecx
	xor edx, edx
	xor ebx, ebx
	mov bl, byte [fat16_SectorsPerCluster]
	shl ebx, 9; Multiply by 512 to get bytes per cluster
	div ebx
	cmp edx, 0
	jg  add_a_bit; If there's a remainder, we need another cluster
	jmp carry_on

add_a_bit:
	add eax, 1

carry_on:
	mov ecx, eax; ECX holds number of clusters that are needed

	;    Allocate all of the clusters required for the amount of bytes we are writing.
	xor  eax, eax
	mov  ax, [fat16_ReservedSectors]; First sector of the first FAT
	add  eax, [fat16_PartitionOffset]; Add the offset to the partition
	mov  edi, hdbuffer0
	mov  esi, edi
	push ecx
	mov  ecx, 1
	call readsectors
	pop  ecx
	xor  edx, edx; cluster we are currently at
	xor  ebx, ebx; cluster marker

	; -----------------------------------------------------------------------------
	; findfirstfreeclust -- Find the first free cluster in the FAT16
	; IN: ESI = Address of the FAT16 sector in memory
	; OUT: DX = Starting cluster ID
	; [startcluster] = Starting cluster ID (saved)

findfirstfreeclust:
	mov   edi, esi; Set EDI to point to the FAT16 sector
	lodsw ; Load word at [esi] into AX and increment ESI
	inc   dx; Increment the counter (cluster ID)
	cmp   ax, 0x0000; Compare AX with 0 (free cluster)
	jne   findfirstfreeclust; If not zero, continue searching
	dec   dx; Decrement DX to get the correct cluster ID
	mov   [startcluster], dx; Save the starting cluster ID
	inc   dx; Move to the next cluster
	mov   bx, dx; Save current cluster ID in BX
	cmp   ecx, 0
	je    clusterdone; If no clusters are needed, finish
	cmp   ecx, 1
	je    clusterdone; If only one cluster needed, finish

findnextfreecluster:
	lodsw ; Load next word in FAT
	inc   dx; Increment cluster ID
	cmp   ax, 0x0000; Check if it's a free cluster
	jne   findnextfreecluster; If not, continue searching
	mov   ax, bx; Restore the start cluster ID
	mov   bx, dx; Save current cluster ID in BX
	stosw ; Store the cluster ID in memory
	mov   edi, esi
	sub   edi, 2; Adjust EDI to point to the start of the FAT
	dec   ecx; Decrement the number of clusters left to find
	cmp   ecx, 1
	jne   findnextfreecluster; Continue if more clusters are needed

clusterdone:
	mov   ax, 0xFFFF
	stosw ; Mark the end of the list
	;     The sector with the updated FAT in hdbuffer0 will be written later
	;     Load the first sector of the file info table
	xor   eax, eax
	mov   eax, [fat16_RootStart]; Get the root directory start sector
	add   eax, [fat16_PartitionOffset]; Add partition offset
	mov   edi, hdbuffer1
	push  edi
	mov   ecx, 1
	call  readsectors
	pop   edi
	mov   ecx, 16; Number of records per sector
	mov   esi, edi

nextrecord:
	dec ecx
	cmp byte [esi], 0x00; Check for empty record
	je  foundfree
	cmp byte [esi], 0xE5; Check for unused record
	je  foundfree
	add esi, 32; Move to the next record
	cmp ecx, 0
	je  os_fat16_file_create_fail
	jmp nextrecord

foundfree:
	;   ESI points to the start of the record
	mov edi, esi
	mov esi, [filename]
	mov ecx, 11

nextchar:
	lodsb
	stosb
	dec   ecx
	cmp   ecx, 0
	jne   nextchar
	xor   eax, eax
	stosb ; LFN Attrib
	stosb ; NT Reserved
	stosw ; Create time
	stosb ; Create time
	stosw ; Create date
	stosw ; Access date
	stosw ; Access time
	stosw ; Modified time
	stosw ; Modified date
	mov   ax, [startcluster]
	stosw ; Starting cluster ID
	mov   eax, [filesize]
	stosd ; File size

	; The updated file record is now in memory at hdbuffer1

	xor   eax, eax
	movzx eax, word [fat16_ReservedSectors]; First FAT sector
	add   eax, [fat16_PartitionOffset]; Add partition offset
	mov   esi, hdbuffer0
	mov   ecx, 1
	call  writesectors

	mov  eax, [fat16_RootStart]; Get the root directory start sector
	add  eax, [fat16_PartitionOffset]; Add partition offset
	mov  esi, hdbuffer1
	mov  ecx, 1
	call writesectors

	jmp os_fat16_file_create_done

os_fat16_file_create_fail:
	stc ; Set carry

os_fat16_file_create_done:
	pop ebx
	pop ecx
	pop edx
	pop edi
	pop esi
	ret

	;            Define data for the file creation
	startcluster dw 0x0000
	filesize     dd 0x00000000
	filename     db 11 dup(0); Ensure enough space for filename

	; -----------------------------------------------------------------------------
	; os_fat16_file_delete -- Delete a file from the hard disk
	; IN: RSI = File name to delete
	; OUT: Carry clear on success, set on failure

os_fat16_file_delete:
	push esi
	push edi
	push edx
	push ecx
	push ebx

	clc ; Clear carry
	xor eax, eax
	mov eax, [fat16_RootStart]; Get the root directory start sector
	add eax, [fat16_PartitionOffset]; Add partition offset
	mov edx, eax; Save the sector value

	;    Convert the file name to FAT format
	mov  edi, os_fat16_file_delete_string
	call os_fat16_filename_convert; Convert the filename to FAT format
	jc   os_fat16_file_delete_error; Fail (Invalid file name)
	mov  esi, edi

	; Read through the root cluster (if file not found bail out)

os_fat16_file_delete_read_sector:
	mov  edi, hdbuffer0
	push edi
	mov  ecx, 1
	call readsectors
	pop  edi
	mov  ebx, 16; Number of records per sector (512 bytes / 32 bytes per record)

os_fat16_file_delete_next_entry:
	cmp byte [edi], 0x00; Check for end of records
	je  os_fat16_file_delete_error

	mov  ecx, 11
	push esi
	repe cmpsb
	pop  esi
	mov  ax, [edi+15]; AX now holds the starting cluster # of the file
	jz   os_fat16_file_delete_found; The file was found

	add edi, 32; Move to the next record
	and edi, -0x20; Align to 32-byte boundary
	dec ebx
	cmp ebx, 0
	jne os_fat16_file_delete_next_entry

	;   At this point we have read through one sector of file names. Load the next sector.
	add edx, 1; Increment sector counter
	mov eax, edx
	jmp os_fat16_file_delete_read_sector

	; Mark the file as deleted (set first byte of file name to 0xE5) and write the sector back to the drive

os_fat16_file_delete_found:
	xor  ebx, ebx
	mov  bx, ax; Save the starting cluster value
	and  edi, -0x20; Align to 32-byte boundary
	mov  byte [edi], 0xE5; Set the first character of the filename to 0xE5
	mov  esi, hdbuffer0
	mov  eax, edx; Retrieve the sector number
	mov  ecx, 1
	call writesectors

	;     Follow cluster chain and set any cluster in the chain to 0x0000 (mark as free)
	xor   eax, eax
	movzx eax, word [fat16_ReservedSectors]; First FAT sector
	add   eax, [fat16_PartitionOffset]; Add partition offset
	mov   edx, eax
	mov   edi, hdbuffer1
	mov   esi, edi
	mov   ecx, 1
	call  readsectors
	xor   eax, eax

os_fat16_file_delete_next_cluster:
	shl  bx, 1
	mov  eax, [esi + ebx*4]
	mov  [esi + ebx*4], eax
	mov  bx, ax
	cmp  ax, 0xFFFF
	jne  os_fat16_file_delete_next_cluster
	mov  eax, edx
	mov  ecx, 1
	call writesectors

	jmp os_fat16_file_delete_done

os_fat16_file_delete_error:
	stc ; Set carry
	xor eax, eax

os_fat16_file_delete_done:
	pop ebx
	pop ecx
	pop edx
	pop edi
	pop esi
	ret

	; Define data for the file deletion
	os_fat16_file_delete_string db 13 dup(0)

	; -----------------------------------------------------------------------------
	; os_fat16_filename_convert -- Change 'test.er' into 'TEST    ER ' as per FAT16
	; IN: ESI = filename string
	; EDI = location to store converted string (carry set if invalid)
	; OUT: All registers preserved
	; NOTE: Must have room for 12 bytes. 11 for the name and 1 for the NULL
	; Need fix for short extensions!

os_fat16_filename_convert:
	push esi
	push edi
	push edx
	push ecx
	push ebx
	push eax

	mov  ebx, edi; Save the string destination address
	call os_string_length
	cmp  ecx, 12; Bigger than name + dot + extension?
	jg   os_fat16_filename_convert_failure; Fail if so
	cmp  ecx, 0
	je   os_fat16_filename_convert_failure; Similarly, fail if zero-char string

	mov edx, ecx; Store string length for now
	xor ecx, ecx; Clear ECX

os_fat16_filename_convert_copy_loop:
	lodsb ; Load byte from [ESI] into AL
	cmp   al, '.'; Check for the dot
	je    os_fat16_filename_convert_extension_found
	stosb ; Store byte in [EDI]
	inc   ecx; Increment character count
	cmp   ecx, edx; Compare with original length
	jg    os_fat16_filename_convert_failure; Fail if length exceeds
	jmp   os_fat16_filename_convert_copy_loop

os_fat16_filename_convert_failure:
	stc ; Set carry for failure
	jmp os_fat16_filename_convert_done

os_fat16_filename_convert_extension_found:
	cmp ecx, 0; Check if the dot was the first character
	je  os_fat16_filename_convert_failure; Fail if so
	cmp ecx, 8; Check if name part is too long
	je  os_fat16_filename_convert_do_extension; Skip spaces if name part is 8 chars

	mov al, ' '; Space for padding

os_fat16_filename_convert_add_spaces:
	stosb ; Store space
	inc   ecx; Increment character count
	cmp   ecx, 8; Check if 8 characters reached
	jl    os_fat16_filename_convert_add_spaces

os_fat16_filename_convert_do_extension:
	;     Process extension, ensuring up to 3 characters
	lodsb ; Load byte from [ESI]
	stosb ; Store byte in [EDI]
	lodsb ; Load next byte
	stosb ; Store byte
	lodsb ; Load next byte
	stosb ; Store byte

	mov byte [edi], 0; Zero-terminate filename

	clc  ; Clear carry for success
	mov  esi, ebx; Restore the start address of the destination string
	call os_string_uppercase; Convert to uppercase

os_fat16_filename_convert_done:
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop edi
	pop esi
	ret

section .data
message db 'Hello, World!', 0
