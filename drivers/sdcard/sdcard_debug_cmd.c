/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "core/include/errors.h"

#include "core/include/device/device.h"
#include "core/include/device/spi.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"

#include "components/vez-shell/include/vez-shell.h"

#include "drivers/sdcard/sdcard.h"
#include "drivers/sdcard/sdcard_spi_impl.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TAG "sdcard"

int sdcard(int argc, char **argv)
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
    DBG(TAG, "sdcard_init(): %s", error_to_str(ret));
    if (ret < 0) goto exit;

    ret = sdcard_read_block(&sdcard, 0, block);
    DBG(TAG, "sdcard_read_block(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    DBG(TAG, "Amount read: %d", ret);
    HEXDUMP(block, sizeof(block));

    exit:
    return E_SUCCESS;
}

SHELL_DECLARE_COMMAND("sdcard", sdcard, "Initializes the SDCARD");

int sdwrite(int argc, char **argv)
{
    uint8_t garbage = strtol(argv[0], NULL, 16);
    DBG(TAG, "Writing %.2x on SDCARD block 0", garbage);
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
    DBG(TAG, "sdcard_init(): %s", error_to_str(ret));
    if (ret < 0) goto exit;

    ret = sdcard_write_block(&sdcard, 0, block);
    DBG(TAG, "sdcard_write_block(): %s", error_to_str(ret));
    if (ret < 0) goto exit;
    DBG(TAG, "Amount written: %d", ret);

    exit:
    return E_SUCCESS;
}

<<<<<<< Updated upstream
SHELL_DECLARE_COMMAND("sdwrite", sdwrite, "Writes a full block to the SDCARD");
=======
// SHELL_DECLARE_COMMAND("sdwrite", sdwrite, "Writes a full block to the SDCARD");
>>>>>>> Stashed changes
