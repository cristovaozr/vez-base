/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_I2C_H_
#define CORE_INCLUDE_DEVICE_I2C_H_

#include "include/device/transaction.h"

#include <stdint.h>

struct i2c_operations;

struct i2c_device {
    /** I2C operation definition */
    const struct i2c_operations * const i2c_ops;
    /** I2C private object which is implementation specific */
    const void * const priv;
};

struct i2c_operations {
    /**
     * @brief Initializes I2C device
     *
     * @param i2c I2C device object
     */
    int32_t (*i2c_init)(const struct i2c_device * const i2c);

    /**
     * @brief Writes to an I2C device
     */
    int32_t (*i2c_write_op)(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout);

    /**
     * @brief Reads from an I2C device
     */
    int32_t (*i2c_read_op)(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout);
};

/*
 * API Definition 
 */

/**
 * @brief Writes to an I2C device as defined in transaction
 *
 * @param i2c I2C device object
 * @param transaction Transaction object
 * @param timeout Timeou in ms
 * @return int32_t Amount of data transmited. Negative number on error
 */
extern int32_t i2c_write(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout);

/**
 * @brief Reads from an I2C device as defined in transaction
 *
 * @param i2c I2C device object
 * @param transaction Transaction object
 * @param timeout Timeou in ms
 * @return int32_t Amount of data received. Negative number on error
 */
extern int32_t i2c_read(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout);

#endif // CORE_INCLUDE_DEVICE_I2C_H_