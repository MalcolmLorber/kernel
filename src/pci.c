// pci.c
//
// Contains implementations of the functions for dealing with PCI
// defined in pci.h

#include "pci.h"
#include "serial.h"

// reads a word from the io port using the pci addressing scheme
uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;

    address = (uint32_t)((lbus << 16) |
                         (lslot << 11) |
                         (lfunc << 8) |
                         (offset & 0xfc) |
                         ((uint32_t)0x80000000));
    outl(PCI_CONFIG_OUT, address);

    // return the contents of the address clipped
    return (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
}

// Check function also prints out stuff
void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
    uint16_t class;

    class = pciConfigReadWord(bus, device, function, 0x08);

    // Print out information about the device
    serial_writestring("0x");
    serial_hexhword(class);
    serial_writestring(" on bus ");
    serial_hexbyte(bus);
    serial_writestring(" device ");
    serial_hexbyte(bus);
    serial_writestring(" function ");
    serial_hexbyte(function);
    serial_writestring("\n");
}

void checkDevice(uint8_t bus, uint8_t device)
{
    uint8_t function = 0;

    uint16_t vendorID = pciConfigReadWord(bus, device, function, 2);
    if (vendorID == 0xFFFF)
    {
        return;
    }

    checkFunction(bus, device, function);
    uint8_t headerType = (pciConfigReadWord(bus, device, function, 0x0C) & 0xff00) >> 8;
    if( (headerType & 0x80) != 0)
    {
        // It is a multi-function device, so check remaining functions
        for(function = 1; function < 8; function++)
        {
            if(pciConfigReadWord(bus, device, function, 2) != 0xFFFF)
            {
                checkFunction(bus, device, function);
            }
        }
    }
}


void checkAllBuses(void)
{
    uint8_t bus;
    uint8_t device;

    for(bus = 0; bus < 255; bus++)
    {
        for(device = 0; device < 32; device++)
        {
            checkDevice(bus, device);
        }
    }
}
