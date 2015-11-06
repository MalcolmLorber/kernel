// pci.h
//
// Contains headers for functions related to dealivg with PCI

#ifndef _PCI_INC
#define _PCI_INC

#include <stdint.h>

#include "asm.h"

#define PCI_CONFIG_ACCESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

uint32_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);


void checkAllBuses(void);

#endif
