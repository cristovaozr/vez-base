/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef LIBS_CRC7_CRC7_H_
#define LIBS_CRC7_CRC7_H_

#include <stdint.h>

/**
 * @brief Calculates CRC7 for poly x^7 + x^3 + 1
 * 
 * @param data Data to calculate the CRC
 * @param size Size of data in bytes
 * @return uint8_t Value of the CRC7
 */
extern uint8_t calc_crc7(const void * const data, uint32_t size);

#endif // LIBS_CRC7_CRC7_H_