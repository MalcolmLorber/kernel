// pci.c
//
// Contains implementations of the functions for dealing with PCI
// defined in pci.h

#include "pci.h"
#include "serial.h"

// reads a word from the io port using the pci addressing scheme
uint32_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
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
    outl(PCI_CONFIG_ACCESS, address);

    // return the contents of the address clipped
    uint32_t data = inl(PCI_CONFIG_DATA);
    if (data != -1)
    {
        serial_writestring("address 0x");
        serial_hexword(address);
        serial_writestring(" returned 0x");
        serial_hexword(data);
        serial_writestring(" on bus ");
        serial_hexbyte(bus);
        serial_writestring(" device ");
        serial_hexbyte(slot);
        serial_writestring(" function ");
        serial_hexbyte(func);
        serial_writestring(" with offset ");
        serial_hexbyte(offset);
        serial_writechar('\n');
    }
    return data;
}

void checkAllBuses(void)
{
    serial_writestring("Enumerating PCI busses\n");
    uint8_t bus;
    uint8_t device;
    uint8_t function;

    for(bus = 0; bus < 255; bus++)
    {
        for(device = 0; device < 32; device++)
        {
            for (function = 0; function < 8; function++)
            {
                if (pciConfigReadWord(bus, device, function, 0) != 0xffffffff)
                {
                    serial_writestring("- - - - - - - - - - - - - - - - - - - -\n");
                    pciConfigReadWord(bus, device, function, 8);
                    serial_writestring("=======================================\n");
                }
            }
        }
    }
}
