/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "drivers/uda1380/uda1380_driver.h"

#include "include/device/device.h"
#include "include/device/i2c.h"
#include "include/device/i2s.h"
#include "include/device/transaction.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"

#include "core/include/errors.h"

#include "components/vez-shell/include/vez-shell.h"

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define TAG "uda1380"

int uda1380(int argc, char **argv)
{
    int32_t ret;
    const struct i2c_device *i2c = device_get_by_name("i2c1");
    const struct i2s_device *i2s3 = device_get_by_name("i2s3");

    if (i2c == NULL) {
        ERROR(TAG, "Could not get I2C device");
        ret = E_DEVICE_NOT_FOUND;
        goto exit;
    }
    if (i2s3 == NULL) {
        ERROR(TAG, "Could not get I2S device");
        ret = E_DEVICE_NOT_FOUND;
        goto exit;
    }

    ret = uda1380_init(i2c);
    if (ret < 0) {
        ERROR(TAG, "Error configuring UDA1380:%s", error_to_str(ret));
        goto exit;
    }

    const float w = 2.0f * M_PI * 1000.0f / 8000.0f;
    for(int i = 0; i < 80000; i++) {
        union {int16_t s; uint16_t u;} sample;
        sample.s = (int16_t)(10000.0f*sinf(w * i));
        uda1380_write_blocking(i2s3, sample.u, sample.u);
    }

    exit:
    return ret;
}

SHELL_DECLARE_COMMAND("uda1380", uda1380, "Tests UDA1380");