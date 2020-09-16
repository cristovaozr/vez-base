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

#include "ulibc/include/utils.h"

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
    memcpy(out, &payload_in[1], size);

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
    ret = spi_write(device->spi_device, 1 + size, payload, 0);
    if (ret >= 0) {
        ret = E_SUCCESS;
    }

    exit:
    return ret;
}

static int32_t read_modify_write(const struct nrf24l01p * const device, uint8_t reg, uint8_t clear_mask, uint8_t set_mask)
{
    int32_t ret;
    uint8_t read_reg[2] = {reg, 0xff}, reg_value[2];
    struct transaction transaction = {
        .transaction_size = 2,
        .write_data = &read_reg,
        .read_data = reg_value
    };

    ret = spi_transact(device->spi_device, &transaction, 0);
    if (ret < 0) {
        goto exit;
    }

    reg_value[1] &= ~clear_mask;
    reg_value[1] |= set_mask;
    uint8_t write_reg[2] = {reg | 0x20, reg_value[1]};
    ret = spi_write(device->spi_device, sizeof(write_reg), write_reg, 0);
    if (ret > 0) { ret = E_SUCCESS; }

    exit:
    return ret;
}

int32_t nrf24l01p_default_setup(const struct nrf24l01p * const device)
{
    int32_t ret;
    const struct configuration {
        uint8_t reg; uint8_t config;
    } configuration[7] = {
        {NRF24L01P_REG_CONFIG,      CONFIG_MASK_RX_DR | CONFIG_MASK_TX_DS | CONFIG_MASK_MAX_RT | CONFIG_PRIM_RX},
        {NRF24L01P_REG_EN_AA,       0x00},
        {NRF24L01P_REG_EN_RXADDR,   EN_RXADDR_ERX_P1 | EN_RXADDR_ERX_P0},
        {NRF24L01P_REG_SETUP_AW,    SETUP_AW_AW_5_BYTES},
        {NRF24L01P_REG_SETUP_RETR,  0x03},
        {NRF24L01P_REG_RF_CH,       0x02},
        {NRF24L01P_REG_RF_SETUP,    RF_SETUP_RF_DR_HIGH | RF_SETUP_RF_PWR_0},
    };

    gpio_write(device->ce_gpio, 0);

    for (int i = 0; i < ARRAY_SIZE(configuration); i++) {
        ret = w_register(device, configuration[i].reg, 1, &configuration[i].config);
        if (ret < 0) {
            goto exit;
        }
    }

    exit:
    return ret;
}

int32_t nrf24l01p_standby_1(const struct nrf24l01p * const device)
{
    // This process goes: Power Down -> Start up (1.5ms at least) -> Standby-I
    int32_t ret = read_modify_write(device, NRF24L01P_REG_CONFIG, 0, CONFIG_PWR_UP);
    if (ret < 0) { goto exit; }

    vTaskDelay(2);
    gpio_write(device->ce_gpio, 0);

    exit:
    return ret;
}

int32_t nrf24l01p_power_down(const struct nrf24l01p * const device)
{
    return read_modify_write(device, NRF24L01P_REG_CONFIG, CONFIG_PWR_UP, 0);
}

int32_t nrf24l01p_get_status(const struct nrf24l01p * const device, uint8_t * const status)
{
    return r_register(device, NRF24L01P_REG_STATUS, 1, status);
}

int32_t nrf24l01p_clear_status_irq(const struct nrf24l01p * const device, uint8_t irqs)
{
    return w_register(device, NRF24L01P_REG_STATUS, 1, &irqs);
}

int32_t nrf24l01p_set_addr_p0(const struct nrf24l01p * const device, uint8_t addr_size, const uint8_t * const addr)
{
    int32_t ret = E_SUCCESS;
    uint8_t size;

    if (addr_size < 3 || addr_size > 5) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if (addr == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    switch(addr_size) {
        case 3: { size = SETUP_AW_AW_3_BYTES; break; }
        case 4: { size = SETUP_AW_AW_4_BYTES; break; }
        case 5: { size = SETUP_AW_AW_5_BYTES; break; }
        default: { ret = E_INVALID_PARAMETER; goto exit; }
    }

    ret = read_modify_write(device, NRF24L01P_REG_SETUP_AW, SETUP_AW_AW_MASK, size);
    if (ret < 0) { goto exit; }
    ret = w_register(device, NRF24L01P_REG_RX_ADDR_P0, addr_size, addr);
    if (ret < 0) { goto exit; }

    exit:
    return ret;
}

int32_t nrf24l01p_get_rx_pw_p0(const struct nrf24l01p * const device, uint8_t *rx_bytes_size)
{
    return r_register(device, NRF24L01P_REG_RX_PW_P0, 1, rx_bytes_size);
}

int32_t nrf24l01p_get_fifo_status(const struct nrf24l01p * const device, uint8_t *fifo_st)
{
    return r_register(device, NRF24L01P_REG_FIFO_STATUS, 1, fifo_st);
}

int32_t nrf24l01p_w_tx_payload(const struct nrf24l01p * const device, uint32_t size, const void * const data)
{
    int32_t ret;
    uint8_t fifo_st;
    uint8_t data_to_write[1 + 32];

    if (size > 32 || data == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if ((ret = r_register(device, NRF24L01P_REG_FIFO_STATUS, 1, &fifo_st)) < 0) { goto exit; }
    if (fifo_st & FIFO_STATUS_TX_FULL) {
        ret = E_TX_QUEUE_FULL;
        goto exit;
    }
    data_to_write[0] = 0b10100000;
    memcpy(&data_to_write[1], data, size);
    if ((ret = spi_write(device->spi_device, 1 + size, data_to_write, 0)) < 0) { goto exit; }
    if (ret > 0) { ret = E_SUCCESS; }

    exit:
    return ret;
}

int32_t nrf24l01p_r_rx_payload(const struct nrf24l01p * const device, uint32_t size, void * const data)
{
    int32_t ret;
    uint8_t fifo_st, data_to_read[1 + 32], data_to_write[1 + 32];
    memset(data_to_write, 0xff, sizeof(data_to_write));
    data_to_write[0] = 0b01100001;

    if (size > 32 || data == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if ((ret = r_register(device, NRF24L01P_REG_FIFO_STATUS, 1, &fifo_st)) < 0) { goto exit; }
    if (fifo_st & FIFO_STATUS_RX_EMPTY) {
        ret = E_RX_QUEUE_EMPTY;
        goto exit;
    }
    struct transaction transaction = {
        .transaction_size = 1 + size,
        .write_data = data_to_write,
        .read_data = data_to_read
    };
    if ((ret = spi_transact(device->spi_device, &transaction, 0)) < 0) { goto exit; }
    memcpy(data, &data_to_read[1], size);
    if (ret > 0) {
        ret = E_SUCCESS;
        goto exit;
    }

    exit:
    return ret;
}

int32_t nrf24l01p_flush_tx(const struct nrf24l01p * const device)
{
    int32_t ret;
    const uint8_t opcode = 0b11100001;
    ret = spi_write(device->spi_device, 1, &opcode, 0);
    if (ret > 0) { ret = E_SUCCESS; }

    return ret;
}

int32_t nrf24l01p_flush_rx(const struct nrf24l01p * const device)
{
int32_t ret;
    const uint8_t opcode = 0b11100010;
    ret = spi_write(device->spi_device, 1, &opcode, 0);
    if (ret > 0) { ret = E_SUCCESS; }

    return ret;
}

void nrf24l01p_transmit(const struct nrf24l01p * const device)
{
    gpio_write(device->ce_gpio, 1);
    // FIXME: wait 10µs instead of 1ms
    vTaskDelay(1);
    gpio_write(device->ce_gpio, 0);
}

void nrf24l01p_enable_receiver(const struct nrf24l01p * const device)
{
    gpio_write(device->ce_gpio, 1);
    vTaskDelay(1);
}

void nrf24l01p_disable_receiver(const struct nrf24l01p * const device)
{
    gpio_write(device->ce_gpio, 0);
}

int32_t nrf24l01p_enable_tx_mode(const struct nrf24l01p * const device)
{
    return read_modify_write(device, NRF24L01P_REG_CONFIG, CONFIG_PRIM_RX, 0);
}

int32_t nrf24l01p_disable_tx_mode(const struct nrf24l01p * const device)
{
    return read_modify_write(device, NRF24L01P_REG_CONFIG, 0, CONFIG_PRIM_RX);
}