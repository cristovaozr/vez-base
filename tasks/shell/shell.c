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
#include "include/device/i2c.h"
#include "include/device/transaction.h"

#include "core/include/errors.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "drivers/nrf24l01p/nrf24l01p.h"
#include "drivers/mpu6050/mpu6050_driver.h"
#include "drivers/uda1380/uda1380_driver.h"
#include "drivers/sdcard/sdcard.h"
#include "drivers/sdcard/sdcard_spi_impl.h"

#include "components/vez-shell/include/vez-shell.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define TAG "SHELL"

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

    // i2c w [hex-addr] [hex-reg] [hexvalue]
    // i2c r [hex-addr] [hex-reg] [size]

    char op_type = argv[0][0];
    uint8_t dev_addr = strtoul(argv[1], NULL, 16);
    uint8_t dev_reg = strtoul(argv[2], NULL, 16);

    switch(op_type) {
        case 'r':
        case 'R': {
            uint32_t size = strtoul(argv[3], NULL, 10);
            uint8_t data[2];
            if (size > sizeof(data)) {
                ret = E_INVALID_PARAMETER;
                goto exit;
            }

            struct i2c_transaction transaction = {
                .i2c_device_addr = dev_addr,
                .i2c_device_reg = dev_reg,
                .transaction_size = size,
                .read_data = &data[0]
            };

            if ((ret = i2c_read(i2c, &transaction, 10000)) < 0) goto exit;
            uprintf("Data read: ");
            for (uint32_t i = 0; i < size; uprintf("%.2x ", data[i++]));
            uprintf("\r\n");
        }
            break;

        case 'w':
        case 'W': {
            uint8_t data[2];
            uint32_t size = strlen(argv[3]);
            if ((size % 2) != 0) {
                ret = E_INVALID_PARAMETER;
                goto exit;
            }
            if (size/2 > sizeof(data)) {
                ret = E_INVALID_PARAMETER;
                goto exit;
            }

            for (int i = 0; i < size; i+=2) {
                char value[3] = {argv[3][0], argv[3][1], '\0'};
                data[i/2] = strtoul(value, NULL, 16);
            }

            struct i2c_transaction transaction = {
                .i2c_device_addr = dev_addr,
                .i2c_device_reg = dev_reg,
                .transaction_size = size/2,
                .write_data = &data[0]
            };

            if ((ret = i2c_write(i2c, &transaction, 10000)) < 0) goto exit;
        }
            break;

        default:
            ret = E_INVALID_PARAMETER;
            goto exit;
    }

    ret = E_SUCCESS;

    exit:
    return ret;
}

static int spi(int argc, char **argv)
{
    int32_t ret = E_SUCCESS;
    const uint8_t original[8] = {'l','o','o','p','b','a','c','k'};
    uint8_t copy[8];

    const struct spi_device *spi = device_get_by_name("spi1");
    if (spi == NULL) {
        ret = E_DEVICE_NOT_FOUND;
        goto exit;
    }

    uprintf("Testing loopback for SPI\r\n");
    struct spi_transaction transaction = {
        .write_size = sizeof(original),
        .write_data = &original[0],
        .read_size = sizeof(copy),
        .read_data = &copy[0]
    };

    uint32_t size = CHOOSE_MIN(sizeof(original), sizeof(copy));

    if ((ret = spi_transact(spi, &transaction, 0)) < 0) {
        goto exit;
    }

    if (memcmp(original, copy, size) == 0) {
        uprintf("Data sent == data received\r\n");
    } else {
        uprintf("Something odd happened! Data sent != data received\r\n");
        HEXDUMP(copy, sizeof(copy));
    }

    exit:
    return ret;
}

static int nrf24l01p(int argc, char **argv)
{

    struct nrf24l01p nrf = {
        .spi_device = device_get_by_name("spi1"),
        .ce_gpio = device_get_by_name("nrf24l01p_ce"),
        .cs_gpio = device_get_by_name("spi1_cs")
    };

    nrf24l01p_default_setup(&nrf);

    return E_SUCCESS;
}

static int sdcard(int argc, char **argv)
{
    const struct sdcard_spi_priv priv = {
        .spi = device_get_by_name("spi1"),
        .cs = device_get_by_name("spi1_cs")
    };
    const struct sdcard sdcard = {
        .priv = &priv
    };
    uint8_t block[512];

    int32_t ret = sdcard_init(&sdcard);
    DBG("SHELL", "sdcard_init(): %s", error_to_str(ret));
    if (ret < 0) goto exit;

    ret = sdcard_read_block(&sdcard, 0, block);
    DBG("SHELL", "sdcard_read_block(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    DBG("SHELL", "Amount read: %d", ret);
    HEXDUMP(block, sizeof(block));

    exit:
    return E_SUCCESS;
}

static int sdwrite(int argc, char **argv)
{
    uint8_t garbage = strtol(argv[0], NULL, 16);
    DBG("SHELL", "Writing %.2x on SDCARD block 0", garbage);
    uint8_t block[512];
    memset(block, garbage, sizeof(block));

    const struct sdcard_spi_priv priv = {
        .spi = device_get_by_name("spi1"),
        .cs = device_get_by_name("spi1_cs")
    };
    const struct sdcard sdcard = {
        .priv = &priv
    };

    int32_t ret = sdcard_init(&sdcard);
    DBG("SHELL", "sdcard_init(): %s", error_to_str(ret));
    if (ret < 0) goto exit;

    ret = sdcard_write_block(&sdcard, 0, block);
    DBG("SHELL", "sdcard_write_block(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    DBG("SHELL", "Amount written: %d", ret);

    exit:
    return E_SUCCESS;
}

static const struct vez_shell_entry cmd_list[] = {
    {"help",    help, "Show help"},
    {"?",       help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"mpu6050", mpu6050, "Reads local acceleration using MPU6050 via I2C"},
    {"uda1380", uda1380, "Writes for ever using UDA1380 via I2S"},
    {"i2c",     i2c, "Reads/write to/from an I2C device"},
    {"spi",     spi, "Tests SPI loopback"},
    {"nrf24l01p", nrf24l01p, "Configures nRF24L01+"},
    {"sdcard",  sdcard, "Tests SDCARD implementation"},
    {"sdwrite", sdwrite, "Writes garbage on SDCARD block 0"},
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

#define SHELL_TASK_SIZE 384
static StackType_t shell_stack[SHELL_TASK_SIZE];
static StaticTask_t shell_tcb;

void declare_shell_task(void)
{
    xTaskCreateStatic(shell_task, "shell", SHELL_TASK_SIZE, NULL, tskIDLE_PRIORITY, shell_stack, &shell_tcb);
}