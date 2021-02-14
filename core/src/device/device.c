/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/device.h"

#include <stdint.h>

struct general_operations {
    int32_t (*general_init)(const void * const device);
};

struct general_device {
    const struct general_operations * const ops;
};

int32_t device_init(const void * const device)
{
    const struct general_device *gdevice = (const struct general_device *)device;
    return gdevice->ops->general_init(device);
}
