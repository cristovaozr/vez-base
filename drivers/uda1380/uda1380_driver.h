/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_UDA_1380_UDA1380_DRIVER_H_
#define DRIVERS_UDA_1380_UDA1380_DRIVER_H_

#include <stdint.h>

#include "include/device/i2c.h"

/**
 * @brief Configures and starts UDA1380 I2S CODEC
 *
 * @param i2c I2C device object
 * @return int32_t E_SUCCESS on success
 */
extern int32_t uda1380_init(const struct i2c_device *i2c);

/**
 * @brief Writes data do UDA1380 I2S blocking
 *
 * @param l_ch Left channel data
 * @param r_ch Right channel data
 */
extern void uda1380_write_blocking(uint16_t l_ch, uint16_t r_ch);

#endif // DRIVERS_UDA_1380_UDA1380_DRIVER_H_