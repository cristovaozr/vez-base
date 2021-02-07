/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_SDCARD_SDCARD_SPI_IMPL_H_
#define DRIVERS_SDCARD_SDCARD_SPI_IMPL_H_

#include "include/device/gpio.h"
#include "include/device/spi.h"

struct sdcard_spi_priv {
    const struct spi_device *spi;
    const struct gpio_device *cs;
};

#endif // DRIVERS_SDCARD_SDCARD_SPI_IMPL_H_