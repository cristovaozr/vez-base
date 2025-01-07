/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "FreeRTOS.h"
#include "task.h"

#include "include/device/device.h"
#include "include/device/spi.h"

#include "drivers/nrf24l01p/nrf24l01p.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"

#include "core/include/errors.h"

#include "components/vez-shell/include/vez-shell.h"

#define TAG "nrf24l"

int nrf24l01p(int argc, char **argv)
{
    struct nrf24l01p nrf = {
        .spi_device = device_get_by_name("spi1"),
        .ce_gpio = device_get_by_name("nrf24l01p_ce"),
        .cs_gpio = device_get_by_name("spi1_cs")
    };

    nrf24l01p_default_setup(&nrf);

    return E_SUCCESS;
}

SHELL_DECLARE_COMMAND("nrf24l01p", nrf24l01p, "Initializes the nRF24L01+");

int nrf24l01p_rx(int argc, char **argv)
{
    struct nrf24l01p nrf = {
        .spi_device = device_get_by_name("spi1"),
        .ce_gpio = device_get_by_name("nrf24l01p_ce"),
        .cs_gpio = device_get_by_name("spi1_cs")
    };
    int32_t ret;

    nrf24l01p_disable_tx_mode(&nrf);
    ret = nrf24l01p_enable_rx_mode(&nrf);
    if (ret < 0) { goto exit; }

    while (1) {
        uint8_t sz;
        ret = nrf24l01p_get_rx_pw_p0(&nrf, &sz);
        if (ret < 0) { goto exit; }
        DBG(TAG, "nrf24l01p_get_rx_pw_0() == %u", sz);
        nrf24l01p_flush_rx(&nrf);

        int c = ugetchar();
        if (c == 'q' || c == 'Q') break;
        vTaskDelay(250);
    }

    exit:

    nrf24l01p_disable_rx_mode(&nrf);
    return ret;
}

SHELL_DECLARE_COMMAND("nrf24l01p_rx", nrf24l01p_rx, "Receives a packet");

int nrf24l01p_tx(int argc, char **argv)
{
    struct nrf24l01p nrf = {
        .spi_device = device_get_by_name("spi1"),
        .ce_gpio = device_get_by_name("nrf24l01p_ce"),
        .cs_gpio = device_get_by_name("spi1_cs")
    };
    int32_t ret;

    nrf24l01p_disable_rx_mode(&nrf);
    ret = nrf24l01p_enable_tx_mode(&nrf);
    if (ret < 0) { goto exit; }

    while (1) {
        ret = nrf24l01p_w_tx_payload(&nrf, 4, "dead");
        DBG(TAG, "ret == %d (%s)", ret, error_to_str(ret));
        int c = ugetchar();
        if (c == 'q' || c == 'Q') break;
        vTaskDelay(250);
    }

    exit:

    nrf24l01p_disable_tx_mode(&nrf);
    return ret;
}

SHELL_DECLARE_COMMAND("nrf24l01p_tx", nrf24l01p_tx, "Transmits a packet");
