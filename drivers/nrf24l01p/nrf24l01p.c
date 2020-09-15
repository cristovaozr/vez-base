/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "drivers/nrf24l01p/nrf24l01p.h"
#include "drivers/nrf24l01p/nrf24l01p_defs.h"

#include "include/device/spi.h"
#include "include/device/transaction.h"
#include "include/device/gpio.h"
#include "include/errors.h"

#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#define ADDR_NOT_VALID(x) ((x) > 0x1d || ((x) >= 0x18 && (x) <= 0x1b))

static int32_t r_register(const struct nrf24l01p * const device, uint8_t addr, uint32_t size, void * const out)
{
    int32_t ret = E_SUCCESS;
    // 1 byte for address, 1 byte for status, up to 5 bytes for data read
    uint8_t payload_in[1 + 5] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t payload_out[1 + 5] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    if (device == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if (ADDR_NOT_VALID(addr)) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    payload_out[0] = addr;
    struct transaction transaction = {
        .transaction_size = 1 + size,
        .write_data = payload_out,
        .read_data = payload_in
    };

    if ((ret = spi_transact(device->spi_device, &transaction, 0)) < 0) {
        goto exit;
    }
    // Copies data including status register
    memcpy(out, payload_in, 1 + size);

    exit:
    return ret;
}

static int32_t w_register(const struct nrf24l01p * const device, uint8_t addr, uint32_t size, const void * const reg)
{
    int32_t ret;
    uint8_t payload[1 + 5];

    if (device == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if (ADDR_NOT_VALID(addr)) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    payload[0] = 0x20 | addr;
    memcpy(&payload[1], reg, size);

    // TODO: In the future when this function returns timeout return here
    ret = spi_write(device->spi_device, payload, 1 + size, 0);
    if (ret >= 0) {
        ret = E_SUCCESS;
    }

    exit:
    return ret;
}

static int32_t read_modify_write(const struct nrf24l01p * const device, uint8_t reg, uint8_t clear_mask, uint8_t set_mask)
{
    int32_t ret;
    uint8_t read_reg = reg | 0x20, reg_value;
    struct transaction transaction = {
        .transaction_size = 1,
        .write_data = &read_reg,
        .read_data = &reg_value
    };

    ret = spi_transact(device->spi_device, &transaction, 0);
    if (ret < 0) {
        goto exit;
    }

    reg_value &= ~clear_mask;
    reg_value |= set_mask;
    uint8_t write_reg[2] = {reg, reg_value};
    ret = spi_write(device->spi_device, write_reg, sizeof(write_reg), 0);

    exit:
    return ret;
}

int32_t nrf24l01p_standby_1(const struct nrf24l01p * const device)
{
    // This process goes: Power Down -> Start up (1.5ms at least) -> Standby-I
    int32_t ret = read_modify_write(device, NRF24L01P_REG_CONFIG, 0, CONFIG_PWR_UP);
    vTaskDelay(2);
    gpio_write(device->ce_gpio, 0);
    return ret;
}

int32_t nrf24l01p_read_status(const struct nrf24l01p * const device, uint8_t * const status)
{
    return r_register(device, NRF24L01P_REG_STATUS, 1, status);
}

int32_t nrf24l01p_power_down(const struct nrf24l01p * const device)
{
    return read_modify_write(device, NRF24L01P_REG_CONFIG, CONFIG_PWR_UP, 0);
}

int32_t nrf24l01p_send_data(const struct nrf24l01p * const device, uint32_t size, const void * const data)
{

}

int32_t nrf24l01p_recv_data(const struct nrf24l01p * const device, uint32_t size, void * const data)
{

}