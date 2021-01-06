#include "drivers/mpu6050/mpu6050_driver.h"

#include "include/device/i2c.h"

#include "include/errors.h"

#include "ulibc/include/utils.h"

#define MPU6050_ADDRESS 0x68

#define MPU6050_ACCEL_X_AXIS    0x3b
#define MPU6050_ACCEL_Y_AXIS    0x3d
#define MPU6050_ACCEL_Z_AXIS    0x3f

#define MPU6050_GYRO_X_AXIS     0x43
#define MPU6050_GYRO_Y_AXIS     0x45
#define MPU6050_GYRO_Z_AXIS     0x47

struct mpu6050_init_config {
    uint8_t mpu6050_reg;
    uint8_t value;
};

static const struct mpu6050_init_config config[4] = {
    {0x1b, 0x18},   /* Register 0x1b (27) Gyroscope Configuration FS_SEL == 3 (+-2000deg/s) */
    {0x1c, 0x00},   /* Register 0x1c (28) Accelerometer Configuration AFS_SEL == 0 (+-2g) */
    {0x23, 0x78},   /* Register 0x23 (35) FIFO Enable XG, YG, ZG and ACCEL enabled */
    {0x6b, 0x01}    /* Register 0x6b (107) Disables sleep */
};

int32_t mpu6050_init(const struct i2c_device * const i2c)
{
    int32_t ret;

    for (int i = 0; i < ARRAY_SIZE(config); i++) {
        struct i2c_transaction transaction = {.i2c_device_addr = MPU6050_ADDRESS, .transaction_size = sizeof(uint8_t)};
        transaction.i2c_device_reg = config[i].mpu6050_reg;
        transaction.write_data = &config[i].value;
        if ((ret = i2c_write(i2c, &transaction, 0)) < 0) goto exit;
    }
    ret = E_SUCCESS;

    exit:
    return ret;
}

int32_t mpu6050_read_gyro_info(const struct i2c_device * const i2c, struct mpu6050_axis *axis)
{
    int32_t ret;

    struct i2c_transaction transaction = {
        .i2c_device_addr = MPU6050_ADDRESS,
        .transaction_size = sizeof(int16_t)
    };

    transaction.i2c_device_reg = MPU6050_GYRO_X_AXIS;
    transaction.read_data = &axis->x_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->x_axis = REV16(axis->x_axis);

    transaction.i2c_device_reg = MPU6050_GYRO_Y_AXIS;
    transaction.read_data = &axis->y_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->y_axis = REV16(axis->y_axis);

    transaction.i2c_device_reg = MPU6050_GYRO_Z_AXIS;
    transaction.read_data = &axis->z_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->z_axis = REV16(axis->z_axis);

    ret = E_SUCCESS;

    exit:
    return ret;
}

int32_t mpu6050_read_accel_info(const struct i2c_device * const i2c, struct mpu6050_axis *axis)
{
    int32_t ret;

    struct i2c_transaction transaction = {
        .i2c_device_addr = MPU6050_ADDRESS,
        .transaction_size = sizeof(int16_t)
    };

    transaction.i2c_device_reg = MPU6050_ACCEL_X_AXIS;
    transaction.read_data = &axis->x_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->x_axis = REV16(axis->x_axis);

    transaction.i2c_device_reg = MPU6050_ACCEL_Y_AXIS;
    transaction.read_data = &axis->y_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->y_axis = REV16(axis->y_axis);

    transaction.i2c_device_reg = MPU6050_ACCEL_Z_AXIS;
    transaction.read_data = &axis->z_axis;
    if ((ret = i2c_read(i2c, &transaction, 0)) < 0) goto exit;
    axis->z_axis = REV16(axis->z_axis);

    ret = E_SUCCESS;

    exit:
    return ret;
}