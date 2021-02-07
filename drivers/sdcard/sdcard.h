/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_SDCARD_SDCARD_H_
#define DRIVERS_SDCARD_SDCARD_H_

#include <include/errors.h>

#include <stdint.h>

struct sdcard {
    /** Private implementation-specific object */
    const void *priv;
};

/**
 * @brief Configures SDCARD as SPI mode
 *
 * @param sdcard SDCARD object
 * @return int32_t E_SUCCESS on success
 */
extern int32_t sdcard_init(const struct sdcard * const sdcard);

/**
 * @brief Reads a block from the SDCARD
 *
 * @param sdcard SDCARD object
 * @param block_number Block number to read from
 * @param blk [output] Block data
 * @return int32_t Number of bytes read in the block (normally 512). Negative on error
 */
extern int32_t sdcard_read_block(const struct sdcard * const sdcard, uint32_t block_number, void * const blk);

/**
 * @brief Writes a block to the SDCARD
 *
 * @param sdcard SDCARD object
 * @param block_number Block number to write to
 * @param blk [in] Block data
 * @return int32_t Number of bytes written on the block (normally 512). Negative on error
 */
extern int32_t sdcard_write_block(const struct sdcard * const sdcard, uint32_t block_number, const void * const blk);

#endif // DRIVERS_SDCARD_SDCARD_H_