/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "FreeRTOS.h"
#include "task.h"

#include "core/include/errors.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "components/vez-shell/include/vez-shell.h"

#include "include/device/device.h"
#include "include/device/usart.h"
#include "include/device/pool_op.h"

#include <string.h>
#include <stdlib.h>

#define TAG "SHELL"

/**
 * @brief Prints help
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
static int help(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    const struct vez_shell_entry *cmds = SHELL_COMMAND_ARRAY_ADDR;

    uprintf("\r\n");
    for(int i = 0; i < SHELL_COMMAND_SIZE; i++) {
        uprintf("%s: %s\r\n", cmds[i].cmd, cmds[i].help_str);
    }

    return E_SUCCESS;
}

static int hexdump(int argc, char **argv)
{
    if (argc < 2) return -1;
    char *endptr = NULL;
    uint32_t addr = strtoul(argv[0], &endptr, 16);
    if (endptr[0] != '\0') return -1;
    uint32_t len = strtoul(argv[1], &endptr, 10);
    if (endptr[0] != '\0') return -1;
    const uint8_t *uaddr = (const uint8_t *)addr;
    HEXDUMP(uaddr, len);

    return E_SUCCESS;
}

static int mem(int argc, char **argv)
{
    int32_t ret;

    if (strncmp("peek", argv[0], 4) == 0) {
        const void *addr = (const void *)strtoul(argv[1], NULL, 16);
        uprintf("Peeking address %p\r\n", addr);
        uprintf("((uint8_t *)addr) == %u\r\n", ((uint8_t *)addr)[0]);
        uprintf("((uint16_t *)addr) == %u\r\n", ((uint16_t *)addr)[0]);
        uprintf("((uint32_t *)addr) == %u\r\n", ((uint32_t *)addr)[0]);
        ret = E_SUCCESS;
        goto exit;

    } else if (strncmp("poke", argv[0], 4) == 0) {
        void *addr = (void *)strtoul(argv[1], NULL, 16);
        const uint32_t value = strtoul(argv[2], NULL, 16);
        uprintf("Writing %x at address %p\r\n", value, addr);

        if (value > 0x0000ffff) {
            ((uint32_t *)addr)[0] = value;
        
        } else if (value > 0x000000ff) {
            ((uint16_t *)addr)[0] = (uint16_t)value;
        
        } else {
            ((uint8_t *)addr)[0] = (uint8_t)value;
        }
        ret = E_SUCCESS;
        goto exit;

    } else {
        uprintf("Please either use PEEK or POKE!\r\n");
        uprintf("E.g.: mem peek hex-addr\r\n");
        uprintf("E.g.: mem poke hex-addr hex-value\r\n");
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    exit:
    return ret;
}

// static int gps_read(int argc, char **argv)
// {
//     (void)argc; (void)argv;
//     int ret = E_SUCCESS;

//     const struct usart_device *gps_usart = device_get_by_name("usart1");
//     const struct usart_device *dbg_usart = device_get_by_name(DEFAULT_USART);
//     if (gps_usart == NULL) {
//         ERROR(TAG, "Could not get usart1 device!");
//         ret = E_DEVICE_NOT_FOUND;
//         goto exit;
//     }

//     while(1) {
//         int c = ugetchar();
//         if (c == 'q' || c == 'Q') break;
//         char line[80];

//         uint32_t size;
//         int32_t lret;
//         if ((lret = usart_pool(gps_usart, POLL_RX_QUEUE_SIZE, &size))) {
//             ERROR(TAG, "usart_pool(): %s", error_to_str(lret));
//             continue;
//         }
//         if (size) {
//             int32_t lsize = usart_read(gps_usart, line, size, 100);
//             usart_write(dbg_usart, line, lsize, 100);
//         }
//     }

//     exit:
//     return ret;
// }

const struct vez_shell_entry basic_entries[] = {
    {"help",    help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"mem",     mem,  "Peeks and Pokes any address"}
    // {"gps_read", gps_read, "Reads GPS input"}
};

SHELL_DECLARE_COMMAND_ARRAY(basic_entries[]);
