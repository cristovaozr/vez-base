/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#define SDCARD_INTERNAL
#include "drivers/sdcard/sdcard_common.h"

#include "libs/crc7/crc7.h"
#include "libs/crc16/crc16.h"

#include <stdint.h>

uint8_t sdcard_calc_crc7(const void * const data, uint32_t size)
{
    return calc_crc7(data, size);
}

uint16_t sdcard_calc_crc16(const void * const data, uint32_t size)
{
    return calc_crc16ccitt(data, size);
}

void sdcard_build_command(uint8_t cmd, uint32_t data, uint8_t *output)
{
    output[0] = (cmd & 0x3f) | 0x40;
    output[1] = (data & 0xff000000) >> 24;
    output[2] = (data & 0x00ff0000) >> 16;
    output[3] = (data & 0x0000ff00) >> 8;
    output[4] = (data & 0x000000ff);
    uint8_t crc7 = sdcard_calc_crc7(&output[0], 5) << 1;
    output[5] =  crc7 | 0x01;
}