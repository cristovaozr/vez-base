/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#include "device/i2s.h"

#include <stdint.h>

int32_t i2s_write(const struct i2s_device * const i2s, uint16_t l_ch, uint16_t r_ch)
{
    return i2s->i2s_ops->i2s_write_op(i2s, l_ch, r_ch);
}