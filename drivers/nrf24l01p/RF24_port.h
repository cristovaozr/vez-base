/*
 * This file is part of a port of the library available at https://github.com/nRF24/RF24
 * to C. Portions of this file were copied and pasted _verbatim_ to facilitate
 * the process of writing it.
 *
 * Done by Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 */

#ifndef NRF24L01P_RF24_H_
#define NRF24L01P_RF24_H_

#include <stdint.h>

#include "include/device/spi.h"

#include "include/errors.h"

#include "ulibc/include/utils.h"

/**
 * @defgroup PALevel Power Amplifier level
 * Power Amplifier level. The units dBm (decibel-milliwatts or dB<sub>mW</sub>)
 * represents a logarithmic signal loss.
 * @see
 * - RF24::setPALevel()
 * - RF24::getPALevel()
 * @{
 */
typedef enum
{
    /**
     * (0) represents:
     * nRF24L01 | Si24R1 with<br>lnaEnabled = 1 | Si24R1 with<br>lnaEnabled = 0
     * :-------:|:-----------------------------:|:----------------------------:
     *  -18 dBm | -6 dBm | -12 dBm
     */
    RF24_PA_MIN = 0,
    /**
     * (1) represents:
     * nRF24L01 | Si24R1 with<br>lnaEnabled = 1 | Si24R1 with<br>lnaEnabled = 0
     * :-------:|:-----------------------------:|:----------------------------:
     *  -12 dBm | 0 dBm | -4 dBm
     */
    RF24_PA_LOW,
    /**
     * (2) represents:
     * nRF24L01 | Si24R1 with<br>lnaEnabled = 1 | Si24R1 with<br>lnaEnabled = 0
     * :-------:|:-----------------------------:|:----------------------------:
     *  -6 dBm | 3 dBm | 1 dBm
     */
    RF24_PA_HIGH,
    /**
     * (3) represents:
     * nRF24L01 | Si24R1 with<br>lnaEnabled = 1 | Si24R1 with<br>lnaEnabled = 0
     * :-------:|:-----------------------------:|:----------------------------:
     *  0 dBm | 7 dBm | 4 dBm
     */
    RF24_PA_MAX,
    /**
     * (4) This should not be used and remains for backward compatibility.
     */
    RF24_PA_ERROR
} rf24_pa_dbm_e;

/**
 * @}
 * @defgroup Datarate datarate
 * How fast data moves through the air. Units are in bits per second (bps).
 * @see
 * - RF24::setDataRate()
 * - RF24::getDataRate()
 * @{
 */
typedef enum
{
    /** (0) represents 1 Mbps */
    RF24_1MBPS = 0,
    /** (1) represents 2 Mbps */
    RF24_2MBPS,
    /** (2) represents 250 kbps */
    RF24_250KBPS
} rf24_datarate_e;

/**
 * @}
 * @defgroup CRCLength CRC length
 * The length of a CRC checksum that is used (if any). Cyclical Redundancy
 * Checking (CRC) is commonly used to ensure data integrity.
 * @see
 * - RF24::setCRCLength()
 * - RF24::getCRCLength()
 * - RF24::disableCRC()
 * @{
 */
typedef enum
{
    /** (0) represents no CRC checksum is used */
    RF24_CRC_DISABLED = 0,
    /** (1) represents CRC 8 bit checksum is used */
    RF24_CRC_8,
    /** (2) represents CRC 16 bit checksum is used */
    RF24_CRC_16
} rf24_crclength_e;

/**
 * @}
 * @brief Driver class for nRF24L01(+) 2.4GHz Wireless Transceiver
 */

typedef struct {
    struct spi_device *rf24_spi;
} RF24;

extern void RF24_ctor(RF24 * const rf24, const struct spi_device * const spi);

extern void RF24_begin(const RF24 * const rf24);

extern int32_t RF24_is_chip_connected(const RF24 * const rf24);

extern void RF24_start_listening(const RF24 * const rf24);

extern void RF24_stop_listening(const RF24 * const rf24);

extern int32_t RF24_available(const RF24 * const rf24);

extern int32_t RF24_read(const RF24 * const rf24, void * const buf, uint8_t len);

extern int32_t RF24_write(const RF24 * const rf24, const void * const buf, uint8_t len);

extern int32_t RF24_open_writing_pipe(const RF24 * const rf24, uint8_t number, const uint8_t * const address);

extern int32_t RF24_available_pipe(const RF24 * const rf24, uint8_t * const pipe_num);

extern int32_t RF24_rx_fifo_full(const RF24 * const rf24);

extern int32_t RF24_is_fifo(const RF24 * const rf24, int32_t about_tx);

extern int32_t RF24_is_fifo_bool(const RF24 * const rf24, int32_t about_tx, int32_t check_empty);

extern int32_t RF24_power_down(const RF24 * const rf24);

extern int32_t RF24_power_up(const RF24 * const rf24);



#endif // NRF24L01P_RF24_H_