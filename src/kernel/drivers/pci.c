#include <kernel/drivers/pci.h>

uint32_t read_pci_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
    uint32_t addr = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, addr);
    return inl(0xCFC);
}