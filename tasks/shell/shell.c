/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "FreeRTOS.h"
#include "task.h"

#include "include/device/device.h"
#include "include/device/spi.h"
#include "include/device/transaction.h"

#include "core/include/errors.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "drivers/nrf24l01p/nrf24l01p.h"
#include "drivers/mpu6050/mpu6050_driver.h"
#include "drivers/uda1380/uda1380_driver.h"

#include "include/device/i2c.h"

#include "components/vez-shell/include/vez-shell.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TAG "shell"

static const struct vez_shell_entry cmd_list[];

/**
 * @brief Prints help
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
static int help(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    uprintf("\r\n");
    for(int i = 0; cmd_list[i].cmd; i++) {
        uprintf("%s: %s\r\n", cmd_list[i].cmd, cmd_list[i].help_str);
    }

    return 0;
}

static int hexdump(int argc, char **argv)
{
    if (argc < 2) return -1;
    char *endptr = NULL;
    uint32_t addr = strtoul(argv[0], &endptr, 16);
    if (endptr[0] != '\0') return -1;
    uint32_t len = strtoul(argv[1], &endptr, 10);
    if (endptr[0] != '\0') return -1;
    const uint8_t *uaddr = (const uint8_t *)addr;
    HEXDUMP(uaddr, len);
    return 0;
}

static int mpu6050(int argc, char **argv)
{
    int32_t ret;
    const struct i2c_device *i2c = device_get_by_name("i2c1");
    if (i2c == NULL) {
        ERROR(TAG, "Could not get I2C device");
        return -1;
    }

    ret = mpu6050_init(i2c);
    DBG(TAG, "mpu6050_init()==%s", error_to_str(ret));
    if (ret != E_SUCCESS) return -1;

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

    return 0;
}

static int uda1380(int argc, char **argv)
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

static int i2c(int argc, char **argv)
{
    int ret;

    const struct i2c_device *i2c = device_get_by_name("i2c1");
    if (i2c == NULL) {
        ERROR(TAG, "Could not obtain I2C device");
        ret = -1;
        goto exit;
    }

    ret = E_SUCCESS;

    exit:
    return ret;
}

static const struct vez_shell_entry cmd_list[] = {
    {"help",    help, "Show help"},
    {"?",       help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"mpu6050", mpu6050, "Reads local acceleration using MPU6050 via I2C"},
    {"uda1380", uda1380, "Writes for ever using UDA1380 via I2S"},
    {"i2c",     i2c, "Reads one or two bytes from I2C"},
    {NULL, NULL, NULL}
};

static void shell_task(void *arg)
{
    (void)arg;

    vez_shell_greeter(NULL);

    while (1) {
        vez_shell_iterate(cmd_list);
    }
}

#define SHELL_TASK_SIZE 256
static StackType_t shell_stack[SHELL_TASK_SIZE];
static StaticTask_t shell_tcb;

void declare_shell_task(void)
{
    xTaskCreateStatic(shell_task, "shell", SHELL_TASK_SIZE, NULL, tskIDLE_PRIORITY, shell_stack, &shell_tcb);
}