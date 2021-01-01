/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_TRANSACTION_H_
#define CORE_INCLUDE_DEVICE_TRANSACTION_H_

#include <stdint.h>

/**
 * @brief Defines a SPI transaction
 */
struct spi_transaction {
    uint32_t transaction_size;
    const void *write_data;
    void *read_data;
};

/**
 * @brief Defines an I2C transaction
 */
struct i2c_transaction {
    uint8_t i2c_device_addr;
    uint8_t i2c_device_reg;
    uint32_t transaction_size;
    const void *write_data;
    void *read_data;
};

#endif // CORE_INCLUDE_DEVICE_TRANSACTION_H_
