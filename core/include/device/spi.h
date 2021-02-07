/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_SPI_H_
#define CORE_INCLUDE_DEVICE_SPI_H_

#include "include/device/pool_op.h"
#include "include/device/transaction.h"

#include <stdint.h>

struct spi_operations;

/**
 * @brief Defines a SPI object
 */
struct spi_device {
    /** SPI operation definition */
    const struct spi_operations * const ops;
    /** SPI private object which is implementation specific */
    const void * const priv;
};

/**
 * @brief Possible operations on SPI objects.
 * Every *_operaitions structure should have the first funptr a init()-like function:
 * it should only receive a "struct *_device * const" and nothing more
 */
struct spi_operations {
    /**
     * @brief Initializes the SPI object
     * @param spi SPI device
     *
     * @return Negative on error.
     */
    int32_t     (*spi_init)(const struct spi_device * const spi);

    /**
     * @brief Writes at most [size] bytes from [data] to the SPI. Negative return means error.
     * @param spi SPI device
     * @param data Data to write to SPI
     * @param size Size in bytes to be written to SPI
     * @param timeout Amount of time, in ms, for timeout
     *
     * @return Amount of data written. Negative on error.
     */
    int32_t     (*spi_write_op)(const struct spi_device * const spi, const void *data, uint32_t size, uint32_t timeout);

    /**
     * @brief Reads at most [size] bytes to [data] from the SPI. Negative return means error.
     *
     * @param spi SPI Device
     * @param data [out] Data to be read
     * @param size Size in bytes to be read from SPI
     * @param timeout Amount of time, in ms, for timeout
     *
     * @return Amount of data read. Negative on error.
     */
    int32_t     (*spi_read_op)(const struct spi_device * const spi, void *data, uint32_t size, uint32_t timeout);

    /**
     * @brief Executes a transaction (write followed by a read) on SPI.
     * @param spi SPI device
     * @param transaction Transaction handler
     * @param timeout Amount of time, in ms, for timeout
     *
     * @return Negative on error
     */
    int32_t     (*spi_transact_op)(const struct spi_device * const spi, struct spi_transaction * const transaction,
        uint32_t timeout);
};

/*
 * API Definition
 */

/**
 * @brief Writes at most [size] bytes from [data] to the SPI.
 *
 * @param spi Object that represents a SPI
 * @param data Data to write to SPI
 * @param size Number of bytes to write to SPI
 * @param timeout Milliseconds to wait to write
 *
 * @return int32_t Number of bytes written or negative on error.
 */
extern int32_t spi_write(const struct spi_device * const spi, const void *data, uint32_t size, uint32_t timeout);

/**
 * @brief Reads at most [size] bytes to [data] from the SPI
 *
 * @param spi Object that represents a SPI
 * @param data Data to read from the SPI
 * @param size Amount of bytes to read
 * @param timeout Milliseconds to wait to read
 * @return int32_t Number of bytes read or negative on error
 */
extern int32_t spi_read(const struct spi_device * const spi, void *data, uint32_t size, uint32_t timeout);

/**
 * @brief Executes a transaction (write and read at the same time) on SPI.
 * @param spi SPI device
 * @param transaction Transaction handler
 * @param timeout Timeout, in ms, to wait for each byte write/read
 *
 * @return Negative on error
 */
extern int32_t spi_transact(const struct spi_device * const spi, struct spi_transaction * const transaction, uint32_t timeout);

#endif // CORE_INCLUDE_DEVICE_SPI_H_
