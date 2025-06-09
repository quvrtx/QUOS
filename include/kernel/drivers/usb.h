#ifndef USB_H
#define USB_H

#include <lib/stdint.h>
#include <kernel/drivers/pci.h>

typedef struct {
    uint32_t vendor_device;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
} USB_DEVICE;

USB_DEVICE find_usb(void);

#endif // USB_H