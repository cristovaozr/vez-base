/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/device.h"
#include "include/device/usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#define DEFAULT_TIMEOUT 100

static char output[80];
static const struct usart_device *usart = NULL;

int uprintf(const char *fmt, ...) {
    va_list ap;

    if (usart == NULL) usart = device_get_by_name(DEFAULT_USART);
    if (usart == NULL) return -1;

    va_start(ap, fmt);
    int ret = vsnprintf(output, sizeof(output), fmt, ap);
    usart_write(usart, output, ret, DEFAULT_TIMEOUT);
    va_end(ap);
    return ret;
}

int uvprintf(const char *fmt, va_list ap) {
    if (usart == NULL) usart = device_get_by_name(DEFAULT_USART);

    int ret = vsnprintf(output, sizeof(output), fmt, ap);
    usart_write(usart, output, ret, DEFAULT_TIMEOUT);
    return ret;
}

int ugetchar(void) {
    int ret;
    uint8_t byte;
    if (usart == NULL) usart = device_get_by_name(DEFAULT_USART);
    if (usart == NULL) return -1;

    if (usart_read(usart, &byte, sizeof(byte), DEFAULT_TIMEOUT) < 0) {
        ret = -1; // Errors and timeout are treated as EOF
    } else {
        ret = byte;
    }

    return ret;
}

int uputchar(int c) {
    if (usart == NULL) usart = device_get_by_name(DEFAULT_USART);
    if (usart == NULL) return -1;

    uint8_t byte = (uint8_t)c;
    usart_write(usart, &byte, sizeof(byte), DEFAULT_TIMEOUT);
    return c;
}

int uputs(const char *s) {
    if (usart == NULL) usart = device_get_by_name(DEFAULT_USART);
    if (usart == NULL) return -1;

    int size = strlen(s);
    usart_write(usart, s, size + 1, DEFAULT_TIMEOUT);
    return size;
}