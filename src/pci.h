// pci.h
//
// Contains headers for functions related to dealivg with PCI

#ifndef _PCI_INC
#define _PCI_INC

#include <stdint.h>

#include "asm.h"

#define PCI_CONFIG_OUT 0xCF8
#define PCI_CONFIG_IN 0xCF8

uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

uint16_t pciCheckVendor(uint8_t bus, uint8_t slot);

void checkAllBuses(void);

#endif
