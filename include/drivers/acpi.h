/*
 acpi.h
 As part of the Avery project
 Created by Maxims Enterprise in 2024
 --------------------------------------------------
 Description: ACPI Driver for Power Management
 Copyright (c) 2024 Maxims Enterprise
*/

#ifndef ACPI_H
#define ACPI_H

#include "utils.h"

extern void debug_shutdown();

extern coreuint32_t *SMI_CMD;
extern coreuint8_t ACPI_ENABLE;
extern coreuint8_t ACPI_DISABLE;
extern coreuint32_t *PM1a_CNT;
extern coreuint32_t *PM1b_CNT;
extern coreuint16_t SLP_TYPa;
extern coreuint16_t SLP_TYPb;
extern coreuint16_t SLP_EN;
extern coreuint16_t SCI_EN;
extern coreuint8_t PM1_CNT_LEN;

struct RSDPtr {
    unsigned char Signature[8];
    unsigned char Checksum;
    char OEMID[6];
    char Revision;
    coreuint32_t *RsdtAddress;
};

struct FACP {
    unsigned char Signature[4];
    coreuint32_t Length;
    unsigned char unneded1[40 - 8];
    coreuint32_t *DSDT;
    unsigned char unneded2[48 - 44];
    coreuint32_t *SMI_CMD;
    unsigned char ACPI_ENABLE;
    unsigned char ACPI_DISABLE;
    unsigned char unneded3[64 - 54];
    coreuint32_t *PM1a_CNT_BLK;
    coreuint32_t *PM1b_CNT_BLK;
    unsigned char unneded4[89 - 72];
    unsigned char PM1_CNT_LEN;
};

extern unsigned int *acpiCheckRSDPtr(unsigned int *ptr);
extern unsigned int *acpiGetRSDPtr();
extern int acpiCheckHeader(unsigned int *ptr, char *sig);
extern int acpiEnable();
extern int init_acpi();
extern void poweroff();

#endif  // ACPI_H
