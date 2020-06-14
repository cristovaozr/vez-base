/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "include/exported.h"

#define SHELL_USART     &usart2
#define DEFAULT_TIMEOUT 100

static char output[80];

int uprintf(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    int ret = vsnprintf(output, sizeof(output), fmt, ap);
    usart_write(SHELL_USART, output, ret, DEFAULT_TIMEOUT);
    va_end(ap);
    return ret;
}

int uvprintf(const char *fmt, va_list ap) {
    int ret = vsnprintf(output, sizeof(output), fmt, ap);
    usart_write(SHELL_USART, output, ret, DEFAULT_TIMEOUT);
    return ret;
}

int ugetchar(void) {
    int ret;
    uint8_t byte;

    if (usart_read(SHELL_USART, &byte, sizeof(byte), DEFAULT_TIMEOUT) < 0) {
        ret = -1; // Errors and timeout are treated as EOF
    } else {
        ret = byte;
    }

    return ret;
}

int uputchar(int c) {
    uint8_t byte = (uint8_t)c;
    usart_write(SHELL_USART, &byte, sizeof(byte), DEFAULT_TIMEOUT);
    return c;
}

int uputs(const char *s) {
    int size = strlen(s);
    usart_write(SHELL_USART, s, size + 1, DEFAULT_TIMEOUT);
    return size;
}