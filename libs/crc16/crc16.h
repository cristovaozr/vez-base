/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef LIBS_CRC16_CRC16_H_
#define LIBS_CRC16_CRC16_H_

#include <stdint.h>

/**
 * @brief Calculate CRC16-CCITT for poly x^16 + x^12 + x^5 + 1
 *
 * @param data Data to calculate the CRC
 * @param size Size of data in bytes
 * @return uint8_t Value of the CRC16-CCITT
 */
extern uint16_t calc_crc16ccitt(const void * const data, uint32_t size);

#endif // LIBS_CRC16_CRC16_H_