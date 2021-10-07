/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "drivers/mpu6050/mpu6050_driver.h"

#include "include/device/device.h"
#include "include/device/i2c.h"
#include "include/device/transaction.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"

#include "core/include/errors.h"

#include "components/vez-shell/include/vez-shell.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>

#define TAG "mpu6050"

int mpu6050(int argc, char **argv)
{
    int32_t ret;
    const struct i2c_device *i2c = device_get_by_name("i2c1");
    if (i2c == NULL) {
        ERROR(TAG, "Could not get I2C device");
        ret = E_DEVICE_NOT_FOUND;
        goto exit;
    }

    ret = mpu6050_init(i2c);
    DBG(TAG, "mpu6050_init()==%s", error_to_str(ret));
    if (ret != E_SUCCESS) {
        ret = E_NOT_INITIALIZED;
        goto exit;
    }

    uprintf("Press 'q' to quit reading\r\n");
    while (1) {
        struct mpu6050_axis axis = {0,0,0};
        mpu6050_read_accel_info(i2c, &axis);
        uprintf("Accel read: x=%d, y=%d, z=%d\r\n", axis.x_axis, axis.y_axis, axis.z_axis);
        memset(&axis, 0x00, sizeof(axis));
        mpu6050_read_gyro_info(i2c, &axis);
        uprintf("Gyro read: x=%d, y=%d, z=%d\r\n", axis.x_axis, axis.y_axis, axis.z_axis);
        int c = ugetchar();
        if (c == 'q' || c == 'Q') break;
        vTaskDelay(250);
    }
    ret = E_SUCCESS;

    exit:
    return ret;
}

SHELL_DECLARE_COMMAND("mpu6050", mpu6050, "Tests the MPU6050");