/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/spi.h"

#include <stdint.h>

int32_t spi_write(const struct spi_device * const spi, const void *data, uint32_t size, uint32_t timeout)
{
    return spi->ops->spi_write_op(spi, data, size, timeout);
}

int32_t spi_read(const struct spi_device * const spi, void *data, uint32_t size, uint32_t timeout)
{
    return spi->ops->spi_read_op(spi, data, size, timeout);
}

int32_t spi_transact(const struct spi_device * const spi, struct spi_transaction * const transaction, uint32_t timeout)
{
    return spi->ops->spi_transact_op(spi, transaction, timeout);
}