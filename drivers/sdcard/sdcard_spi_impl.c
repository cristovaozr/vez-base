/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#include "drivers/sdcard/sdcard_spi_impl.h"
#include "drivers/sdcard/sdcard.h"

#define SDCARD_INTERNAL
#include "drivers/sdcard/sdcard_common.h"

#include "include/errors.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "include/device/gpio.h"
#include "include/device/spi.h"

#include <stdint.h>
#include <stddef.h>

// Every SDCARD command is 6 bytes long
#define DEFAULT_SIZE_CMD 6

/** Block size for the SDCARD device */
#define DEFAULT_BLOCK_SIZE 512

/** SDCARD Start of transmission byte */
#define SDCARD_SOT 0xfe

/** Number maximum of bytes that the SDCARD takes to start block reading */
#define SOT_MAX_DELAY_IN_BYTES 32

// R1 bits
#define R1_READY_STATE              0x00
#define R1_IDLE_STATE               0x01
#define R1_ERASE_RESET              0x02
#define R1_ILLEGAL_COMMAND          0x04
#define R1_CRC_ERROR                0x08
#define R1_ERASE_SEQUENCE_ERROR     0x10
#define R1_ADDRESS_ERROR            0x20
#define R1_PARAMETER_ERROR          0x40

#define TAG "SDCARD"

static const uint8_t idle_80clock[10] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static uint32_t sdcard_shift_count = 0;

/**
 * @brief Sends a SDCARD command (6 bytes) and receives a R1 response
 *
 * @param sdcard SDCARD object
 * @param data [in] Data of the SDCARD command (must be at least 6 bytes)
 * @param resp [out] R1 response from the SDCARD
 * @return int32_t E_SUCCESS on success. On error resp is unreliable
 */
static int32_t send_cmd_and_get_r1_response(const struct sdcard * const sdcard, const uint8_t *data, uint8_t *resp)
{
    int32_t ret = E_SUCCESS;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;
    uint8_t rxd;

    if (sdcard == NULL || resp == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    gpio_write(priv->cs, GPIO_LOW);
    spi_write(priv->spi, data, DEFAULT_SIZE_CMD, 0);

    int retry = 8;
    do {
        spi_read(priv->spi, &rxd, sizeof(rxd), 0);
        if ((rxd & 0x80) == 0) break;
    } while (--retry);

    gpio_write(priv->cs, GPIO_HIGH);

    if (retry == 0) {
        ret = E_TIMEOUT;
        goto exit;
    }

    *resp = rxd;
    
    exit:
    return ret;
}

/**
 * @brief Sends a SDCARD command (6 bytes) and receives a R2 response
 *
 * @param sdcard SDCARD object
 * @param data [in] Data of the SDCARD command (must be at least 6 bytes)
 * @param resp [out] R2 response from the SDCARD
 * @return int32_t E_SUCCESS on success. On error resp is unreliable
 */
static int32_t send_cmd_and_get_r2_response(const struct sdcard * const sdcard, const uint8_t *data, uint8_t *resp)
{
    int32_t ret = E_SUCCESS;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;
    uint8_t rxd;

    if (sdcard == NULL || resp == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    gpio_write(priv->cs, GPIO_LOW);
    spi_write(priv->spi, data, DEFAULT_SIZE_CMD, 0);

    int retry = 8;
    do {
        spi_read(priv->spi, &rxd, sizeof(rxd), 0);
        if ((rxd & 0x80) == 0) break;
    } while (--retry);

    if (retry == 0) {
        ret = E_TIMEOUT;
        goto exit;
    }

    resp[0] = rxd;
    spi_read(priv->spi, &rxd, sizeof(rxd), 0);
    resp[1] = rxd;
    
    exit:
    gpio_write(priv->cs, GPIO_HIGH);
    return ret;
}

/**
 * @brief Sends a SDCARD command (6 bytes) and receives a R3 or R7 response
 *
 * @param sdcard SDCARD object
 * @param data [in] Data of the SDCARD command (must be at least 6 bytes)
 * @param resp [out] R3 or R7 response from the SDCARD
 * @return int32_t E_SUCCESS on success. On error resp is unreliable
 */
static int32_t send_cmd_and_get_r3_r7_response(const struct sdcard * const sdcard, const uint8_t *data, uint8_t *resp)
{
    int32_t ret = E_SUCCESS;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;
    uint8_t rxd;

    if (sdcard == NULL || resp == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    gpio_write(priv->cs, GPIO_LOW);
    spi_write(priv->spi, data, DEFAULT_SIZE_CMD, 0);

    int retry = 8;
    do {
        spi_read(priv->spi, &rxd, sizeof(rxd), 0);
        if ((rxd & 0x80) == 0) break;
    } while(--retry);

    if (retry == 0) {
        ret = E_TIMEOUT;
        goto exit;
    }
    resp[0] = rxd;
    spi_read(priv->spi, &resp[1], sizeof(uint32_t), 0);

    exit:
    gpio_write(priv->cs, GPIO_HIGH);
    return ret;
}

int32_t sdcard_init(const struct sdcard * const sdcard)
{
    int32_t ret = E_SUCCESS;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;
    uint8_t r1, r3r7[5];
    uint8_t cmd[DEFAULT_SIZE_CMD];
    uint32_t hcs = 0x00000000;
    int32_t need_to_set_block_size = FALSE;

    // Sends 80 clock cycles with CS low
    gpio_write(priv->cs, GPIO_HIGH);
    spi_write(priv->spi, idle_80clock, sizeof(idle_80clock), 0);

    // Sends CMD0
    sdcard_build_command(0, 0x00000000, cmd);
    ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
    DBG(TAG, "r1_response(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    if (r1 != R1_IDLE_STATE) {
        ret = E_HARDWARE_CONFIG_FAILED;
        goto exit;
    }

    // Sends CMD8
    sdcard_build_command(8, 0x0000015a, cmd);
    ret = send_cmd_and_get_r3_r7_response(sdcard, cmd, &r3r7[0]);
    DBG(TAG, "r1_r3_response(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    HEXDUMP(r3r7, sizeof(r3r7));
    if (r3r7[0] != R1_IDLE_STATE) {
        ret = E_HARDWARE_CONFIG_FAILED;
        goto exit;
    }
    if ((r3r7[3] & 0x0f) != 0x01) {
        ret = E_HARDWARE_CONFIG_FAILED;
        goto exit;
    }
    if (r3r7[4] != 0x5a) {
        ret = E_HARDWARE_CONFIG_FAILED;
        goto exit;
    }
    hcs = 0x40000000;

    // Sends CMD55 and CMD41
    int retry = 2048;
    do {
        sdcard_build_command(55, 0, cmd);
        ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
        if (ret == E_TIMEOUT) continue;
        if (r1 != R1_IDLE_STATE) continue;

        sdcard_build_command(41, hcs, cmd);
        ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
        if (ret == E_TIMEOUT) continue;
        if (r1 == R1_READY_STATE) break;
    } while(--retry);

    // Sends CMD58
    sdcard_build_command(58, 0, cmd);
    ret = send_cmd_and_get_r3_r7_response(sdcard, cmd, r3r7);
    DBG(TAG, "r1_r3_response(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    HEXDUMP(r3r7, sizeof(r3r7));
    if (r3r7[0] != R1_READY_STATE) {
        ret = E_HARDWARE_CONFIG_FAILED;
        goto exit;
    }
    if (IS_BIT_CLEAR(r3r7[0], 0x40)) {
        // SDCARD is byte-oriented
        need_to_set_block_size = TRUE;
    }

    if (need_to_set_block_size) {
        sdcard_build_command(16, DEFAULT_BLOCK_SIZE, cmd);
        ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
        DBG(TAG, "r1_r3_response(): %s", error_to_str(ret));
        if (ret < 0) goto exit;
        sdcard_shift_count = 9;
    }

    exit:
    return ret;
}

int32_t sdcard_read_block(const struct sdcard * const sdcard, uint32_t block_number, void * const blk)
{
    int32_t ret = E_SUCCESS;
    uint8_t cmd[DEFAULT_SIZE_CMD];
    uint8_t r1, rxd;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;
    uint16_t crc16;

    // Sends CMD17 to read single block
    if (sdcard_shift_count) block_number <<= sdcard_shift_count;
    sdcard_build_command(17, block_number, cmd);
    ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
    if (ret < 0) goto exit;
    if (r1 != R1_READY_STATE) {
        ret = E_INVALID_HARDWARE;
        goto exit;
    }

    gpio_write(priv->cs, GPIO_LOW);

    int32_t retry = SOT_MAX_DELAY_IN_BYTES;
    do {
        ret = spi_read(priv->spi, &rxd, sizeof(rxd), 0);
        if (ret < 0) goto exit;
        if (rxd == SDCARD_SOT) break;
    } while (--retry);
    if (retry == 0) {
        ret = E_TIMEOUT;
        goto exit;
    }

    int32_t amount_read = spi_read(priv->spi, blk, 512, 0);
    if (amount_read < 0) goto exit;
    ret = spi_read(priv->spi, &crc16, sizeof(crc16), 0);
    if (ret < 0) goto exit;
    crc16 = REV16(crc16); // SDCARD is BIG ENDIAN therefore must revert for this is little endian

    // Checks CRC16
    uint16_t block_crc16 = sdcard_calc_crc16(blk, 512);

    DBG(TAG, "crc16 == %.4x, block_crc16 = %.4x", crc16, block_crc16);
    if (block_crc16 != crc16) {
        ret = E_INVALID_CRC;
        goto exit;
    }

    ret = amount_read;

    exit:
    gpio_write(priv->cs, GPIO_HIGH);
    return ret;
}

int32_t sdcard_write_block(const struct sdcard * const sdcard, uint32_t block_number, const void * const blk)
{
    int32_t ret = E_SUCCESS;
    uint8_t cmd[DEFAULT_SIZE_CMD];
    uint8_t r1, r2[2], sbt, bsy;
    const struct sdcard_spi_priv *priv = (const struct sdcard_spi_priv *)sdcard->priv;

    // Sends CMD24 to write single block
    if (sdcard_shift_count) block_number <<= sdcard_shift_count;
    sdcard_build_command(24, block_number, cmd);
    ret = send_cmd_and_get_r1_response(sdcard, cmd, &r1);
    if (ret < 0) goto exit;
    if (r1 != R1_READY_STATE) {
        ret = E_INVALID_HARDWARE;
        goto exit;
    }

    uint16_t crc16 = sdcard_calc_crc16(blk, 512);

    gpio_write(priv->cs, GPIO_LOW);

    // Sends Start Block Token to begin transmission. The variable rxd is reused
    sbt = SDCARD_SOT;
    spi_write(priv->spi, &sbt, sizeof(sbt), 0);

    // Sends block data to SDCARD
    int32_t amount_written = spi_write(priv->spi, blk, 512, 0);
    if (amount_written < 0) {
        ret = amount_written;
        goto exit;
    }
    ret = spi_write(priv->spi, &crc16, sizeof(crc16), 0);
    if (ret < 0) goto exit;

    // Waits for data being written to the SDCARD
    int32_t retry = 2048;
    do {
        ret = spi_read(priv->spi, &bsy, sizeof(bsy), 0);
        if (ret < 0) goto exit;
        if (bsy == 0xff) break;
    } while(--retry);

    if (retry == 0) {
        ret = E_TIMEOUT;
        goto exit;
    }

    // Sends CMD13 to fetch status
    sdcard_build_command(13, 0, cmd);
    ret = send_cmd_and_get_r2_response(sdcard, cmd, r2);   
    if (ret < 0) goto exit;
    if (r2[1]) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    ret = amount_written;

    exit:
    return ret;
}