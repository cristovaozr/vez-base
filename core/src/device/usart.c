/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/usart.h"

#include <stdint.h>

int32_t usart_write(const struct usart_device * const usart, const void *data, uint32_t size, uint32_t timeout)
{
    return usart->ops->usart_write_op(usart, data, size, timeout);
}

int32_t usart_read(const struct usart_device  * const usart, void *data, uint32_t size, uint32_t timeout)
{
    return usart->ops->usart_read_op(usart, data, size, timeout);
}

int32_t usart_pool(const struct usart_device  * const usart, enum poll_op op, void *answer)
{
    return usart->ops->usart_poll_op(usart, op, answer);
}