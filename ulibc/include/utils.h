/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef ULIBC_INCLUDE_UTILS_H_
#define ULIBC_INCLUDE_UTILS_H_

/** Calculates the size of a pre-allocated array */
#define ARRAY_SIZE(x) (sizeof(x)/(sizeof(x[0])))

/** Reverts 16 bits values */
#define REV16(x) (uint16_t)(((x) & 0xff00) >> 8 | ((x) & 0x00ff) << 8)

/** Reverts 32 bits values */
#define REV32(x) (uint32_t)(((x) & 0xff000000) >> 24 | ((x) & 0xff0000) >> 8 | ((x) & 0xff00) << 8 | ((x) & 0xff) << 24)

/** Retreives the maximum of two integers */
#define CHOOSE_MAX(x, y) ((x)>(y)?(x):(y))

/** Retreives the minimum of two integers */
#define CHOOSE_MIN(x, y) ((x)>(y)?(y):(x))

#endif // ULIBC_INCLUDE_UTILS_H_