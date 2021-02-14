/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_NRF24L01P_NRF24L01P_H_
#define DRIVERS_NRF24L01P_NRF24L01P_H_

#include "include/device/spi.h"

#include "drivers/nrf24l01p/nrf24l01p_defs.h"

#include <stdint.h>

struct nrf24l01p {
    const struct spi_device * const spi_device;
    const struct gpio_device * const ce_gpio;
    const struct gpio_device * const cs_gpio;
};

/**
 * @brief Configures nRF24L01+ to the driver default configuration
 *
 * @param device nRF24L01+ device definition
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_default_setup(const struct nrf24l01p * const device);

/**
 * @brief Puts nRF24L01+ device in Standby-I state
 *
 * @param device nRF24L01+ device definition
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_standby_1(const struct nrf24l01p * const device);

/**
 * @brief Puts nRF24L01+ device in Power Down state
 *
 * @param device nRF24L01+ device definition
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_power_down(const struct nrf24l01p * const device);

/**
 * @brief Reads status from nRF24L01+
 *
 * @param device nRF24L01+ device definition
 * @param status [out] contains status register
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_get_status(const struct nrf24l01p * const device, uint8_t * const status);

/**
 * @brief Clears nRF24L01+ status IRQ flags
 * 
 * @param device nRF24L01+ device definition
 * @param irqs IRQ flags to clear. Can be either STATUS_RX_DR or STATUS_TX_DS or STATUS_MAX_RT or any mix
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_clear_status_irq(const struct nrf24l01p * const device, uint8_t irqs);

/**
 * @brief Sets PIPE0 address
 *
 * @param device nRF24L01+ device definition
 * @param addr_size PIPE0 address size of 3, 4 or 5 bytes
 * @param addr Address
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_set_addr_p0(const struct nrf24l01p * const device, uint8_t addr_size, const uint8_t * const addr);

/**
 * @brief Gets packet size as received by PIPE0
 *
 * @param device nRF24L01+ device definition.
 * @param rx_bytes_size [out] Size in bytes of the received packet.
 * @return int32_t Negative value on error.
 */
extern int32_t nrf24l01p_get_rx_pw_p0(const struct nrf24l01p * const device, uint8_t *rx_bytes_size);

/**
 * @brief Gets nRF24L01+ FIFO status
 *
 * @param device nRF24L01+ device definition
 * @param fifo_st [out] FIFO status. Can be explored by AND and bits defined by FIFO_STATUS_* flags
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_get_fifo_status(const struct nrf24l01p * const device, uint8_t *fifo_st);

/**
 * @brief Writes a packet to TX queue
 *
 * @param device nRF24L01+ device definition
 * @param size Size, in bytes, of data to write. Up to 32 bytes
 * @param data Data to write
 * @return int32_t Negative value on error. If queue is full returns with E_TX_QUEUE_FULL
 */
extern int32_t nrf24l01p_w_tx_payload(const struct nrf24l01p * const device, uint32_t size, const void * const data);

/**
 * @brief Reads a packet from RX queue
 *
 * @param device nRF24L01+ device definition
 * @param size Size, in bytes, of data to read. Up to 32 bytes
 * @param data [out] Data read from RX queue
 * @return int32_t Negative value on error. If queue is empty returns with E_RX_QUEUE_EMPTY
 */
extern int32_t nrf24l01p_r_rx_payload(const struct nrf24l01p * const device, uint32_t size, void * const data);

/**
 * @brief Clears TX queue
 *
 * @param device nRF24L01+ device definition
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_flush_tx(const struct nrf24l01p * const device);

/**
 * @brief Clears RX queue
 *
 * @param device nRF24L01+ device definition
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_flush_rx(const struct nrf24l01p * const device);

/**
 * @brief Toggles CE so that the nRF24L01+ transmits
 * 
 * @param device nRF24L01+ device definition
 */
extern void nrf24l01p_transmit(const struct nrf24l01p * const device);

extern void nrf24l01p_enable_receiver(const struct nrf24l01p * const device);

extern void nrf24l01p_disable_receiver(const struct nrf24l01p * const device);

extern int32_t nrf24l01p_enable_tx_mode(const struct nrf24l01p * const device);

extern int32_t nrf24l01p_disable_tx_mode(const struct nrf24l01p * const device);

#endif // DRIVERS_NRF24L01P_NRF24L01P_H_
