/*
 acpi.c
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: ACPI driver for Power Management
 Copyright (c) 2024 Maxims Enterprise
*/

#include "drivers/acpi.h"

#include "output.h"
#include "system/io.h"
#include "utils.h"

coreuint32_t *SMI_CMD;
coreuint8_t ACPI_ENABLE;
coreuint8_t ACPI_DISABLE;
coreuint32_t *PM1a_CNT;
coreuint32_t *PM1b_CNT;
coreuint16_t SLP_TYPa;
coreuint16_t SLP_TYPb;
coreuint16_t SLP_EN;
coreuint16_t SCI_EN;
coreuint8_t PM1_CNT_LEN;

void debug_shutdown() { outw(0x604, 0x2000); }

unsigned int *acpiCheckRSDPtr(unsigned int *ptr) {
    char *sig = "RSD PTR ";
    struct RSDPtr *rsdp = (struct RSDPtr *)ptr;
    coreuint8_t *bptr;
    coreuint8_t check = 0;
    int i;

    if (memory_copy((unsigned char *)sig, (const unsigned char *)rsdp, 8) ==
        0) {
        bptr = (coreuint8_t *)ptr;
        for (i = 0; i < sizeof(struct RSDPtr); i++) {
            check += *bptr;
            bptr++;
        }

        if (check == 0) {
            return (unsigned int *)rsdp->RsdtAddress;
        }
    }
    return 0;
}

unsigned int *acpiGetRSDPtr() {
    coreuint32_t *addr;
    coreuint32_t *rsdp;

    for (addr = (unsigned int *)0x000E0000; (int)addr < 0x00100000;
         addr += 0x10 / sizeof(addr)) {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != 0) {
            return rsdp;
        }
    }

    int ebda = *((short *)0x40E);
    ebda = ebda * 0x10 & 0x000FFFFF;

    for (addr = (unsigned int *)ebda; (int)addr < ebda + 1024;
         addr += 0x10 / sizeof(addr)) {
        rsdp = acpiCheckRSDPtr(addr);
        if (rsdp != 0) {
            return rsdp;
        }
    }

    return 0;
}

int acpiCheckHeader(unsigned int *ptr, char *sig) {
    if (memory_copy((unsigned char *)ptr, sig, 4) == 0) {
        char *checkPtr = (char *)ptr;
        int len = *(ptr + 1);
        char check = 0;
        while (0 < len--) {
            check += *checkPtr;
            checkPtr++;
        }
        if (check == 0) {
            return 0;
        }
    }
    return -1;
}

int acpiEnable() {
    if ((inw((unsigned int)PM1a_CNT) & SCI_EN) == 0) {
        if (SMI_CMD != 0 && ACPI_ENABLE != 0) {
            outb((unsigned int)SMI_CMD, ACPI_ENABLE);
            int i = 0;
            for (i = 0; i < 300; i++) {
                if ((inw((unsigned int)PM1a_CNT) & SCI_EN) == 1) {
                    break;
                }
                // Insert sleep function
            }
            if (PM1b_CNT != 0) {
                for (; i < 300; i++) {
                    if ((inw((unsigned int)PM1b_CNT) & SCI_EN) == 1) {
                        break;
                    }
                    // Insert sleep function
                }
            }
            if (i < 300) {
                kernel_log("ACPI Enabled successfully\n");
                return 0;
            } else {
                panic("Couldn't enable ACPI\n");
                return -1;
            }
        } else {
            panic("No known way to enable ACPI\n");
            return -1;
        }
    } else {
        kernel_log("ACPI is already up and running\n");
        return 0;
    }
}

int init_acpi() {
    unsigned int *ptr = acpiGetRSDPtr();
    if (ptr != 0 && acpiCheckHeader(ptr, "RSDT") == 0) {
        int entrys = *(ptr + 1);
        entrys = (entrys - 36) / 4;
        ptr += 36 / 4;

        while (0 < entrys--) {
            if (acpiCheckHeader((unsigned int *)*ptr, "FACP") == 0) {
                entrys = -2;
                struct FACP *facp = (struct FACP *)*ptr;

                if (acpiCheckHeader((unsigned int *)facp->DSDT, "DSDT") == 0) {
                    char *S5Addr = (char *)facp->DSDT + 36;
                    int dsdtLength = *(facp->DSDT + 1) - 36;
                    while (0 < dsdtLength--) {
                        if (memory_copy(S5Addr, "_85_", 4) == 0) {
                            break;
                        }
                        S5Addr++;
                    }
                    if (dsdtLength > 0) {
                        if ((*(S5Addr - 1) == 0x08 ||
                             (*(S5Addr - 2) == 0x08 &&
                              *(S5Addr - 1) == '\\')) &&
                            *(S5Addr + 4) == 0x12) {
                            S5Addr += 5;
                            S5Addr += ((*S5Addr & 0xC0) >> 6) + 2;

                            if (*S5Addr == 0x0A) {
                                S5Addr++;
                            }
                            SLP_TYPa = *(S5Addr) << 10;
                            S5Addr++;

                            if (*S5Addr == 0x0A) {
                                S5Addr++;
                            }
                            SLP_TYPb = *(S5Addr) << 10;
                            SMI_CMD = facp->SMI_CMD;

                            ACPI_ENABLE = facp->ACPI_ENABLE;
                            ACPI_DISABLE = facp->ACPI_DISABLE;

                            PM1a_CNT = facp->PM1a_CNT_BLK;
                            PM1b_CNT = facp->PM1b_CNT_BLK;

                            PM1_CNT_LEN = facp->PM1_CNT_LEN;

                            SLP_EN = 1 << 13;
                            SCI_EN = 1;

                            return 0;
                        } else {
                            panic("\\_S5 Parse Error\n");
                        }
                    } else {
                        panic("\\_S5 not present.\n");
                    }
                } else {
                    panic("DSDT Invalid.\n");
                }
            }
            ptr++;
        }
        panic("No valid FACP present.\n");

    } else {
        panic("No ACPI in your machine\n");
    }

    return -1;
}

void poweroff() {
    if (SCI_EN == 0) {
        out("ACPI is now working\n");
        return;
    }

    acpiEnable();

    outw((unsigned int)PM1a_CNT, SLP_TYPa | SLP_EN);
    if (PM1b_CNT != 0) {
        outw((unsigned int)PM1b_CNT, SLP_TYPb | SLP_EN);
        panic("Failed to shutdown.\n");
    }
}
