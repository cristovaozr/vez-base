/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef INCLUDE_DEVICE_I2S_H_
#define INCLUDE_DEVICE_I2S_H_

#include <stdint.h>

struct i2s_operations;

struct i2s_device {
    /** I2C operation definition */
    const struct i2s_operations * const i2s_ops;
    /** I2C private object which is implementation specific */
    const void * const priv;
};

struct i2s_operations {
    /**
     * @brief Initializes I2C device
     *
     * @param i2c I2C device object
     */
    int32_t (*i2s_init)(const struct i2s_device * const i2s);

    /**
     * @brief Writes to an I2C device
     */
    int32_t (*i2s_write_op)(const struct i2s_device * const i2s, uint16_t l_ch, uint16_t r_ch);
};

/*
 * API Definition 
 */

/**
 * @brief Writes to an I2S device
 *
 * @param i2s I2S device object
 * @param l_ch Left channel data
 * @param r_ch Right channel data
 *
 * @return int32_t Amount of data transmited. Negative number on error
 */
extern int32_t i2s_write(const struct i2s_device * const i2s, uint16_t l_ch, uint16_t r_ch);

#endif // INCLUDE_DEVICE_I2S_H_