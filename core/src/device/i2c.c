/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/i2c.h"

#include <stdint.h>

int32_t i2c_write(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout)
{
    return i2c->i2c_ops->i2c_write_op(i2c, transaction, timeout);
}

int32_t i2c_read(const struct i2c_device * const i2c, const struct i2c_transaction *transaction, uint32_t timeout)
{
    return i2c->i2c_ops->i2c_read_op(i2c, transaction, timeout);
}
