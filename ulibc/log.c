/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "ulibc/include/log.h"

#include "ulibc/include/ustdio.h"

#include <stdarg.h>

#define DBG_COLOR "\e[1m\e[32m"
#define INFO_COLOR "\e[1m\e[36m"
#define WARN_COLOR "\e[1m\e[33m"
#define ERROR_COLOR "\e[1m\e[31m"
#define END_COLOR "\e[0m"

void ulog(enum log_level level, const char *tag, const char *fmt, ...)
{
    va_list ap;
    const char *start, *greeting;

    switch(level) {
        case INFO_LVL:
            start = INFO_COLOR;
            greeting = "INFO ";
            break;
        
        case WARN_LVL:
            start = WARN_COLOR;
            greeting = "WARN ";
            break;

        case ERROR_LVL:
            start = ERROR_COLOR;
            greeting = "ERROR";
            break;

        default:
        case DEBUG_LVL:
            start = DBG_COLOR;
            greeting ="DEBUG";
            break;
    }

    uprintf("%s[%s][%s]: ", start, greeting, tag);

    va_start(ap, fmt);
    uvprintf(fmt, ap);
    va_end(ap);

    uprintf(END_COLOR "\r\n");
}

void hex_ulog(const void *data, uint32_t len)
{
    const uint8_t *udata = (const uint8_t *)data;

    uprintf("Hexdump of [%p]:\r\n", data);
    if (len >= 16) {
        for(uint32_t i = 0; i < len; i+=16) {
            uprintf("| %.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x  "
                      "%.2x %.2x %.2x %.2x %.2x %.2x %.2x %.2x |" "\r\n",
                udata[i +  0], udata[i +  1], udata[i +  2], udata[i +  3],
                udata[i +  4], udata[i +  5], udata[i +  6], udata[i +  7],
                udata[i +  8], udata[i +  9], udata[i + 10], udata[i + 11],
                udata[i + 12], udata[i + 13], udata[i + 14], udata[i + 15]);
        }
    }

    uint32_t remain = len % 16;
    if (remain == 0) return;

    uprintf("| ");
    for(uint32_t i = 0; i < 16; i++) {
        if (i == 8) uprintf(" ");                
        if (i < remain) uprintf("%.2x ", udata[len / 16 + i]);
        else uprintf(".. ");
    }
    uprintf("|\r\n");
}