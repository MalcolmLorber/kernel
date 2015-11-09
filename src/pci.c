// pci.c
//
// Contains implementations of the functions for dealing with PCI
// defined in pci.h

#include "pci.h"
#include "serial.h"

void pciSerialInfo(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)device;
    uint32_t lfunc = (uint32_t)function;
    address = (uint32_t)((lbus << 16) |
                         (lslot << 11) |
                         (lfunc << 8) |
                         (offset & 0xfc) |
                         ((uint32_t)0x80000000));
    outl(PCI_CONFIG_ACCESS, address);

    uint32_t data = inl(PCI_CONFIG_DATA);

    serial_writestring("address 0x");
    serial_hexword(address);
    serial_writestring(" returned 0x");
    serial_hexword(data);
    serial_writestring(" on bus ");
    serial_hexbyte(bus);
    serial_writestring(" device ");
    serial_hexbyte(device);
    serial_writestring(" function ");
    serial_hexbyte(function);
    serial_writestring(" with offset ");
    serial_hexbyte(offset);
    serial_writechar('\n');
}

// reads a word from the io port using the pci addressing scheme
uint32_t pciConfigReadWord(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t ldev = (uint32_t)device;
    uint32_t lfunc = (uint32_t)func;

    address = (uint32_t)((lbus << 16) |
                         (ldev << 11) |
                         (lfunc << 8) |
                         (offset & 0xfc) |
                         ((uint32_t)0x80000000));
    outl(PCI_CONFIG_ACCESS, address);

    // return the contents of the address clipped
    uint32_t data = inl(PCI_CONFIG_DATA);
    return data;
}

void checkAllBuses(void)
{
    serial_writestring("Enumerating PCI busses\n");
    uint8_t bus;
    uint8_t device;
    uint8_t function;

    for (bus = 0; bus < 255; bus++)
    {
        for (device = 0; device < 32; device++)
        {
            for (function = 0; function < 8; function++)
            {
                if (pciConfigReadWord(bus, device, function, 0) != 0xffffffff)
                {
                    pciSerialInfo(bus, device, function, 0);
                    serial_writestring("- - - - - - - - - - - - - - - - - - - -\n");
                    pciSerialInfo(bus, device, function, 8);
                    serial_writestring("=======================================\n");
                }
            }
        }
    }
}

// pciFindAHCI enumerates the pci bus to find the AHCI controller
void* pciFindAHCI()
{
    uint8_t bus;
    uint8_t device;
    uint8_t function;
    void* baseAddr = 0;

    for (bus = 0; bus < 255; bus++)
    {
        for (device = 0; device <32; device++)
        {
            for (function = 0; function < 8; function++)
            {
                uint16_t classID = pciConfigReadWord(bus, device, function, 8) >> 16;
                if (classID != 0xffff)
                {
                    serial_hexhword(classID);
                    serial_writechar('\n');
                }
                // This class ID is for any
                if (classID == 0x0106)
                {
                    if (baseAddr != 0)
                    {
                        serial_writestring("Found too many SATA devices!\n");
                        return baseAddr;
                    }
                    serial_writestring("Found AHCI address\n");
                    pciSerialInfo(bus, device, function, 0);
                    pciSerialInfo(bus, device, function, 8);
                    serial_writestring("AHCI Base address: 0x");
                    baseAddr = (void*) pciConfigReadWord(bus, device, function, 0x24);
                    serial_hexword((uint32_t) baseAddr);
                    serial_writechar('\n');
                }
            }
        }
    }

    if (baseAddr == 0)
    {
        serial_writestring("Could not find an ahci device\n");
    }
    return baseAddr;
}
