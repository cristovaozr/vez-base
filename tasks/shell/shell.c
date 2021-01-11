/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "FreeRTOS.h"
#include "task.h"

#include "include/exported.h"
#include "include/device/spi.h"
#include "include/device/transaction.h"

#include "core/include/errors.h"

#include "ulibc/include/ustdio.h"
#include "ulibc/include/log.h"
#include "ulibc/include/utils.h"

#include "drivers/nrf24l01p/nrf24l01p.h"
#include "drivers/mpu6050/mpu6050_driver.h"
#include "drivers/uda1380/uda1380_driver.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

#define SHELL_BANNER "> "

struct function_list {
    const char *cmd;
    int (*fp)(int, char **);
    const char *help_str;
};

static const struct function_list cmd_list[];

/**
 * @brief Parses a line and chooses a command to execute
 * 
 * @param line 
 */
static void process_line(char *line);

/**
 * @brief Greeter that shows information on Shell start
 */
static void greeter(void);

/**
 * @brief Variable that stores a line to process
 */
static char line[80];

void shell_task(void *arg)
{
    (void)arg;
    int pos = 0;

    greeter();
    uprintf(SHELL_BANNER);

    while(1) {
        int c = ugetchar();
        if (c == -1) continue;

        switch (c) {
        case '\b': // Backspace
        case 0x7f:
            if (pos > 0) uputchar(c);
            pos = ((pos - 1) < 0) ? 0 : pos - 1;
            break;

        case '\n':
        case '\r':
            uprintf("\r\n");
            line[pos] = '\0'; // EOL
            process_line(line);
            pos = 0;
            uprintf(SHELL_BANNER); // New line
            break;
        
        default:
            // If line buffer is full stop printint chars
            if (pos > sizeof(line)) break;
            uputchar(c);
            line[pos++] = c;
            break;
        }
    }
}

void greeter(void)
{
    uprintf("VEZ Tiny Shell\r\n");
    uprintf("Build date: %s, %s\r\n", __DATE__ , __TIME__);
}

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

    uprintf("\r\n");
    for(int i = 0; cmd_list[i].cmd; i++) {
        uprintf("%s: %s\r\n", cmd_list[i].cmd, cmd_list[i].help_str);
    }

    return 0;
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
    return 0;
}

static int mpu6050(int argc, char **argv)
{
    struct mpu6050_axis axis;
    int32_t ret;

    ret = mpu6050_init(&i2c1);
    uprintf("mpu6050_init()==%d\r\n", ret);
    uprintf("mpu6050_read_accel_info()==%d\r\n", ret);

    uprintf("Press 'q' to quit readint\r\n");
    while (1) {
        mpu6050_read_accel_info(&i2c1, &axis);
        uprintf("Accel read: x=%d, y=%d, z=%d\r\n", axis.x_axis, axis.y_axis, axis.z_axis);
        mpu6050_read_gyro_info(&i2c1, &axis);
        uprintf("Gyro read: x=%d, y=%d, z=%d\r\n", axis.x_axis, axis.y_axis, axis.z_axis);
        int c = ugetchar();
        if (c == 'q' || c == 'Q') break;
        vTaskDelay(250);
    }

    return 0;
}

static int uda1380(int argc, char **argv)
{
    int32_t ret;

    ret = uda1380_init(&i2c1);
    if (ret < 0) {
        return -1;
    }

    const float w = 2.0f * M_PI * 1000.0f / 8000.0f;
    for(int i = 0; i < 80000; i++) {
        union {int16_t s; uint16_t u;} sample;
        sample.s = (int16_t)(10000.0f*sinf(w * i));
        uda1380_write_blocking(&i2s3, sample.u, sample.u);
    }

    return 0;
}

static const struct function_list cmd_list[] = {
    {"help", help, "Show help"},
    {"?", help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"mpu6050", mpu6050, "Reads local acceleration using MPU6050 via I2C"},
    {"uda1380", uda1380, "Writes for ever using UDA1380 via I2S"},
    {NULL, NULL, NULL}
};

static void process_line(char *line)
{
    char *saveptr, *cmd, *argv[8];
    int argc;

    // Extracts command name
    cmd = strtok_r(line, " ", &saveptr);
    if (cmd == NULL) return;

    // Extracts arguments
    for(argc = 0; argc < 8; argc++) {
        char *token = strtok_r(NULL, " ", &saveptr);
        if (token == NULL) break;
        argv[argc] = token;
    }

    // Looks for a command in the command list
    for(int i = 0; cmd_list[i].cmd; i++) {
        if(strcmp(cmd, cmd_list[i].cmd) == 0) {
            int ret = cmd_list[i].fp(argc, argv);
            if (ret) {
                uprintf("Error executing command: %s\r\n", cmd);
            }

            return; // Found command. Executed (?) and now quit
        }
    }
   uprintf("Command not foud: %s\r\n", cmd);
}