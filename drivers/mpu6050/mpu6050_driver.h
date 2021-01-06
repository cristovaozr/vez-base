#ifndef DRIVERS_MPU6050_MPU6050_DRIVER_H_
#define DRIVERS_MPU6050_MPU6050_DRIVER_H_

#include "include/device/i2c.h"

#include <stdint.h>

struct mpu6050_axis {
    int16_t x_axis;
    int16_t y_axis;
    int16_t z_axis;
};

/**
 * @brief Configures MPU6050 gyroscope and accelerometer MEMS sensor
 *
 * @param i2c I2C object
 * @return int32_t E_SUCCESS on success
 */
extern int32_t mpu6050_init(const struct i2c_device * const i2c);

/**
 * @brief Reads gyroscope information from MPU6050 MEMS sensor
 *
 * @param i2c I2C object
 * @param axis [out] mpu6050_axis object where values are stored
 * @return int32_t E_SUCCESS on success
 */
extern int32_t mpu6050_read_gyro_info(const struct i2c_device * const i2c, struct mpu6050_axis *axis);

/**
 * @brief Reads accelerometer information from MPU6050 MEMS sensor
 *
 * @param i2c I2C object
 * @param axis [out] mpu6050_axis object where values are stored
 * @return int32_t E_SUCCESS on success
 */
extern int32_t mpu6050_read_accel_info(const struct i2c_device * const i2c, struct mpu6050_axis *axis);

#endif // DRIVERS_MPU6050_MPU6050_DRIVER_H_