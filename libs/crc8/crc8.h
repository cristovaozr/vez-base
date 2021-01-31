/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef LIBS_CRC8_CRC8_H_
#define LIBS_CRC8_CRC8_H_

#include <stdint.h>

/**
 * @brief Calculate CRC8-CCITT for poly x^8 + x^2 + x + 1
 *
 * @param data Data to calculate the CRC
 * @param size Size of data in bytes
 * @return uint8_t Value of the CRC8-CCITT
 */
extern uint8_t calc_crc8ccitt(const void * const data, uint32_t size);

#endif // LIBS_CRC8_CRC8_H_