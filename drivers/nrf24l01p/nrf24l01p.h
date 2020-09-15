/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_NRF24L01P_NRF24L01P_H_
#define DRIVERS_NRF24L01P_NRF24L01P_H_

#include "include/device/spi.h"

#include <stdint.h>

struct nrf24l01p {
    const struct spi_device * const spi_device;
    const struct gpio_device * const ce_gpio;
};

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
extern int32_t nrf24l01p_read_status(const struct nrf24l01p * const device, uint8_t * const status);

/**
 * @brief Sends payload (up to 32 bytes)
 * 
 * @param device nRF24L01+ device definition
 * @param size Payload size in bytes
 * @param data [in] Payload data to be sent
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_send_data(const struct nrf24l01p * const device, uint32_t size, const void * const data);

/**
 * @brief Receives payload (up to 32 bytes)
 * 
 * @param device nRF24L01+ device definition
 * @param size Payload size in bytes
 * @param data [out] Payload data to be read
 * @return int32_t Negative value on error
 */
extern int32_t nrf24l01p_recv_data(const struct nrf24l01p * const device, uint32_t size, void * const data);

#endif // DRIVERS_NRF24L01P_NRF24L01P_H_
