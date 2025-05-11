#ifndef PCI_H
#define PCI_H

#include <lib/stdint.h>
#include <kernel/drivers/io_port.h>

uint32_t read_pci_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t write_pci_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

#endif //PCI_H