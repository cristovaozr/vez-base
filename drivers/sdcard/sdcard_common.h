/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_SDCARD_SDCARD_COMMON_H_
#define DRIVERS_SDCARD_SDCARD_COMMON_H_

#ifndef SDCARD_INTERNAL
#warning "You should not import sdcard_common.h directly!"
#endif // SDCARD_INTERNAL

#include <stdint.h>

/**
 * @brief Calculates CRC7 for SDCard implementation
 *
 * @param data Data to calculate the CRC
 * @param size Size of data in bytes
 * @return uint8_t Value of the CRC
 */
extern uint8_t sdcard_calc_crc7(const void * const data, uint32_t size);

/**
 * @brief Calculates CRC16 for SDCARD implementation
 *
 * @param data Data to calculate the CRC
 * @param size Size of data in bytes
 * @return uint8_t Value of the CRC
 */
extern uint16_t sdcard_calc_crc16(const void * const data, uint32_t size);

/**
 * @brief Builds a SDCARD command
 *
 * @param cmd Command number
 * @param data Auxiliary data for the command
 * @param output [out] Buffer contaning the command
 */
extern void sdcard_build_command(uint8_t cmd, uint32_t data, uint8_t *output);

#endif // DRIVERS_SDCARD_SDCARD_COMMON_H_