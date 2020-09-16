/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/spi.h"

#include <stdint.h>

int32_t spi_write(const struct spi_device * const spi, uint32_t size, const void *data, uint32_t timeout)
{
    return spi->ops->spi_write_op(spi, size, data, timeout);
}

int32_t spi_transact(const struct spi_device * const spi, struct transaction * const transaction, uint32_t timeout)
{
    return spi->ops->spi_transact_op(spi, transaction, timeout);
}