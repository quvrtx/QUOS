#ifndef PCI_H
#define PCI_H

#include <kernel/drivers/io_port.h>

uint32_t read_pci_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
void write_pci_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t value);

#endif //PCI_H