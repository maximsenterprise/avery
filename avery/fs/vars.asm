global fat16_FatStart
global fat16_TotalSectors
global fat16_DataStart
global fat16_RootStart
global fat16_PartitionOffset
global fat16_ReservedSectors
global fat16_RootDirEnts
global fat16_SectorsPerFat
global fat16_BytesPerSector
global fat16_SectorsPerCluster
global fat16_Fats
global secbuffer1
global hdbuffer1
global hdbuffer0
global hd1_maxlba
global hd1_size
global os_SystemVariables
global secbuffer0

fat16_FatStart:   dd 0x00000000
fat16_TotalSectors:  dd 0x00000000
fat16_DataStart:  dd 0x00000000
fat16_RootStart:  dd 0x00000000
fat16_PartitionOffset:  dd 0x00000000
fat16_ReservedSectors:  dw 0x0000
fat16_RootDirEnts:  dw 0x0000
fat16_SectorsPerFat:  dw 0x0000
fat16_BytesPerSector:  dw 0x0000
fat16_SectorsPerCluster: db 0x00
fat16_Fats:   db 0x00
os_SystemVariables equ 0x0000000000110000

secbuffer1 equ 0x0000000000080A00

hdbuffer1 equ 0x0000000000078000

hdbuffer0 equ 0x000000000007000

secbuffer0 equ 0x0000000000080800

hd1_maxlba  equ os_SystemVariables + 40

hd1_size  equ os_SystemVariables + 132
