/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "core/include/errors.h"

#include "core/include/device/device.h"
#include "core/include/device/i2c.h"
#include "core/include/device/spi.h"
#include "core/include/device/transaction.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "components/vez-shell/include/vez-shell.h"

#include <stdlib.h>
#include <string.h>

#define TAG "dev"

int i2c(int argc, char **argv)
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

SHELL_DECLARE_COMMAND("i2c", i2c, "Reads/writes to the I2C bus");

int spi(int argc, char **argv)
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

SHELL_DECLARE_COMMAND("spi", spi, "Reads/writes to the SPI bus");