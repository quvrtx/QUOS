#include <drivers/usb.h>

USB_DEVICE find_usb(void) {
    USB_DEVICE device = {0};

    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                uint32_t vendor_device = read_pci_config(bus, slot, func, 0x00);
                uint16_t vendor_id = vendor_device & 0xFFFF;
                if (vendor_id == 0xFFFF) continue;
                
                uint16_t device_id = vendor_device >> 16;
                uint8_t class_code = (read_pci_config(bus, slot, func, 0x08) >> 24) & 0xFF;
                
                if (class_code == 0x0C) {
                    device.vendor_device = vendor_device;
                    device.vendor_id = vendor_id;
                    device.device_id = device_id;
                    device.bus = bus;
                    device.slot = slot;
                    device.func = func;
                    
                    return device;
                }
            }
        }
    }
    
    return device;
}