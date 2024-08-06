align 16
db    'DEBUG: INIT_HDD '
align 16

extern secbuffer0
extern readsectors
extern fat16_FatStart
extern fat16_TotalSectors
extern fat16_DataStart
extern fat16_RootStart
extern fat16_PartitionOffset
extern fat16_ReservedSectors
extern fat16_RootDirEnts
extern fat16_SectorsPerFat
extern fat16_BytesPerSector
extern fat16_SectorsPerCluster
extern hd1_size
extern hd1_maxlba
extern fat16_Fats

global hdd_setup

hdd_setup:
	;    Read first sector (MBR) into memory
	xor  eax, eax
	mov  edi, secbuffer0
	push edi
	mov  ecx, 1
	call readsectors
	pop  edi

	cmp byte [0x00000000F030], 0x01; Did we boot from a MBR drive
	jne hdd_setup_no_mbr; If not, then we already have the correct sector

	;   Grab the partition offset value for the first partition
	mov eax, [edi+0x01C6]
	mov [fat16_PartitionOffset], eax

	;    Read the first sector of the first partition
	mov  edi, secbuffer0
	push edi
	mov  ecx, 1
	call readsectors
	pop  edi

hdd_setup_no_mbr:
	;   Get the values we need to start using FAT16
	mov ax, [edi+0x0b]
	mov [fat16_BytesPerSector], ax; This will probably be 512
	mov al, [edi+0x0d]
	mov [fat16_SectorsPerCluster], al; This will be 128 or less (Max cluster size is 64KiB)
	mov ax, [edi+0x0e]
	mov [fat16_ReservedSectors], ax
	mov [fat16_FatStart], eax
	mov al, [edi+0x10]
	mov [fat16_Fats], al; This will probably be 2
	mov ax, [edi+0x11]
	mov [fat16_RootDirEnts], ax
	mov ax, [edi+0x16]
	mov [fat16_SectorsPerFat], ax

	;   Find out how many sectors are on the disk
	xor eax, eax
	mov ax, [edi+0x13]
	cmp ax, 0x0000
	jne lessthan65536sectors
	mov eax, [edi+0x20]

lessthan65536sectors:
	mov [fat16_TotalSectors], eax

	;   Calculate the size of the drive in MiB
	xor eax, eax
	mov eax, [fat16_TotalSectors]
	mov [hd1_maxlba], eax
	shr eax, 11; eax = eax * 512 / 1048576
	mov [hd1_size], eax; in mebibytes

	;   Calculate FAT16 info
	xor eax, eax
	xor ebx, ebx
	mov ax, [fat16_SectorsPerFat]
	shl ax, 1; quick multiply by two
	add ax, [fat16_ReservedSectors]
	mov [fat16_RootStart], ax
	mov bx, [fat16_RootDirEnts]
	shr bx, 4; bx = (bx * 32) / 512
	add bx, ax; BX now holds the datastart sector number
	mov [fat16_DataStart], bx

	ret
