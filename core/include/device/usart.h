/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_USART_H_
#define CORE_INCLUDE_DEVICE_USART_H_

#include "include/device/pool_op.h"

#include <stdint.h>

struct usart_operations;

/**
 * @brief Defines a object that is a GPIO
 */
struct usart_device {
    const struct usart_operations * const ops;
    /**
     * @brief Private data from the object. Normally contains arch-dependent stuff
     */
    const void * const priv;
};

/**
 * @brief Possible operations on GPIO objects.
 * Every *_operaitions structure should have the first funptr a init()-like function:
 * it should only receive a "struct *_device * const" and nothing more
 */
struct usart_operations {
    /**
     * @brief Initializes the GPIO object
     */
    int32_t (*usart_init)(const struct usart_device * const usart);

    /**
     * @brief Writes at most [size] bytes from [data] to the USART. Negative return means error.
     */
    int32_t    (*usart_write_op)(const struct usart_device * const usart, const void *data, uint32_t size,
        uint32_t timeout);

    /**
     * @brief Reads at most [size] bytes from [data] to the USART. Negative return means error.
     */
    int32_t (*usart_read_op)(const struct usart_device * const usart, void *data, uint32_t size, uint32_t timeout);

    /**
     * @brief Polls USART for a specific operation. Negative number means error.
     */
    int32_t    (*usart_poll_op)(const struct usart_device * const usart, enum poll_op op, void *answer);
};

/*
 * API Definition
 */

/**
 * @brief Writes at most [size] bytes from [data] to the USART.
 *
 * @param usart Object that represents a USART
 * @param data Data to write to USART
 * @param size Number of bytes to write to USART
 * @param timeout Milliseconds to wait to write
 * @return int32_t Number of bytes written or negative on error.
 */
extern int32_t usart_write(const struct usart_device * const usart, const void *data, uint32_t size, uint32_t timeout);

/**
 * @brief Reads at most [size] bytes to [data] from the USART.
 * 
 * @param usart Object that represents a USART
 * @param data Data to read from USART
 * @param size Number of bytes re read from USART
 * @param timeout Milliseconds to wait to read
 * @return int32_t Number of bytes read or negative on error.
 */
extern int32_t usart_read(const struct usart_device  * const usart, void *data, uint32_t size, uint32_t timeout);

/**
 * @brief Polls USART for some event
 * 
 * @param usart Object that represents a USART
 * @param op Operation as defined by enum poll_op
 * @return int32_t Answer to the operation. Negative on error.
 */
extern int32_t usart_pool(const struct usart_device  * const usart, enum poll_op op, void *answer);

#endif // CORE_INCLUDE_DEVICE_GPIO_H_
