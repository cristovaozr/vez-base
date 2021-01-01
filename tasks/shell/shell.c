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

#include <string.h>
#include <stdlib.h>

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

static int spitst(int argc, char **argv)
{
    const uint8_t spitest[8] = {0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xca, 0xfe};
    uint8_t spiresp[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    struct transaction trans = {
        .transaction_size = sizeof(spitest),
        .write_data = spitest,
        .read_data = spiresp
    };

    spi_transact(&spi2, &trans, 0);
    uprintf("Transmited:\r\n");
    HEXDUMP(spitest, sizeof(spitest));
    uprintf("Received:\r\n");
    HEXDUMP(spiresp, sizeof(spiresp));

    return 0;
}

static int nrftst(int argc, char **argv)
{
    struct nrf24l01p nrf24l01p = {
        .spi_device = &spi2
    };

    uint8_t status;
    int32_t ret = nrf24l01p_get_status(&nrf24l01p, &status);

    if (ret < 0) {
        uprintf("Error reading data from nRF24L01+: %d\r\n", ret);
        goto exit;
    }

    uprintf("nrf24l01p status = %.2x\r\n", status);

    exit:
    return ret;
}

static int nrftst2(int argc, char **argv)
{
    const char *msg = "Hello nRF24L01+";
    int msglen = strlen(msg);
    const struct nrf24l01p nrf24l01p = { .spi_device = &spi2, .ce_gpio = &nrf24l01p_ce };
    uint8_t fifo_st;
    int32_t ret;

    ret = nrf24l01p_default_setup(&nrf24l01p);
    uprintf("nrf24l01p_default_setup() == %d\r\n", ret);
    ret = nrf24l01p_standby_1(&nrf24l01p);
    uprintf("nrf24l01p_standby_1() == %d\r\n", ret);

    // First payload to write
    ret = nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);
    ret = nrf24l01p_w_tx_payload(&nrf24l01p, msglen, msg);
    uprintf("nrf24l01p_w_tx_payload() == %d\r\n", ret);

    // Second payload to write
    ret = nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);
    ret = nrf24l01p_w_tx_payload(&nrf24l01p, msglen, msg);
    uprintf("nrf24l01p_w_tx_payload() == %d\r\n", ret);

    // Third payload to write
    ret = nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);
    ret = nrf24l01p_w_tx_payload(&nrf24l01p, msglen, msg);
    uprintf("nrf24l01p_w_tx_payload() == %d\r\n", ret);

    // Fourth payload to write (should now return E_TX_QUEUE_FULL)
    ret = nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);
    ret = nrf24l01p_w_tx_payload(&nrf24l01p, msglen, msg);
    uprintf("nrf24l01p_w_tx_payload() == %d\r\n", ret);

    ret = nrf24l01p_enable_tx_mode(&nrf24l01p);
    uprintf("nrf24l01p_enable_tx_mode() == %d\r\n", ret);
    nrf24l01p_transmit(&nrf24l01p);
    ret = nrf24l01p_disable_tx_mode(&nrf24l01p);
    uprintf("nrf24l01p_disable_tx_mode() == %d\r\n", ret);

    // FIFO should now have two used slots
    ret = nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);

    ret = nrf24l01p_flush_tx(&nrf24l01p);
    uprintf("nrf24l01p_flush_tx() = %d\r\n", ret);

    // After TX fifo flush there should be no TX packet on queue and TX queue should be empty
    nrf24l01p_get_fifo_status(&nrf24l01p, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status() = %d, fifo_st = %x\r\n", ret, fifo_st);

    return 0;
}

static int nrftst3(int argc, char **argv)
{
    const char *msg = "Hello nRF24L01+";
    char recvmsg[33];
    int msglen = strlen(msg);
    const struct nrf24l01p nrf24l01p_sender = { .spi_device = &spi2, .ce_gpio = &nrf24l01p_ce };
    const struct nrf24l01p nrf24l01p_receiv = { .spi_device = &spi1, .ce_gpio = &nrf24l01p_ce2 };
    uint8_t fifo_st, rx_pipe_size;
    int32_t ret;

    ret = nrf24l01p_default_setup(&nrf24l01p_sender);
    uprintf("nrf24l01p_default_setup(&nrf24l01p_sender) == %d\r\n", ret);
    ret = nrf24l01p_default_setup(&nrf24l01p_receiv);
    uprintf("nrf24l01p_default_setup(&nrf24l01p_receiv) == %d\r\n", ret);

    ret = nrf24l01p_standby_1(&nrf24l01p_sender);
    uprintf("nrf24l01p_standby_1(nrf24l01p_sender) == %d\r\n", ret);
    ret = nrf24l01p_standby_1(&nrf24l01p_receiv);
    uprintf("nrf24l01p_standby_1(nrf24l01p_receiv) == %d\r\n", ret);

    ret = nrf24l01p_w_tx_payload(&nrf24l01p_sender, msglen, msg);
    uprintf("nrf24l01p_w_tx_payload(&nrf24l01p_sender) == %d\r\n", ret);
    nrf24l01p_enable_receiver(&nrf24l01p_receiv);
    ret = nrf24l01p_enable_tx_mode(&nrf24l01p_sender);
    nrf24l01p_transmit(&nrf24l01p_sender);
    ret = nrf24l01p_disable_tx_mode(&nrf24l01p_sender);
    nrf24l01p_disable_receiver(&nrf24l01p_receiv);

    ret = nrf24l01p_get_fifo_status(&nrf24l01p_receiv, &fifo_st);
    uprintf("nrf24l01p_get_fifo_status(&nrf24l01p_receiv) == %d, fifo_st == %x\r\n", ret, fifo_st);

    ret = nrf24l01p_get_rx_pw_p0(&nrf24l01p_receiv, &rx_pipe_size);
    uprintf("nrf24l01p_get_rx_pw_p0(&nrf24l01p_receiv) == %d, rx_pipe_size == %x\r\n", ret, rx_pipe_size);

    memset(recvmsg, 0x00, sizeof(recvmsg));
    ret = nrf24l01p_r_rx_payload(&nrf24l01p_receiv, rx_pipe_size, recvmsg);
    uprintf("nrf24l01p_r_rx_payload(&nrf24l01p_receiv) == %d\r\n", ret);
    uprintf("Received: %s\r\n", recvmsg);

    return 0;
}

static const struct function_list cmd_list[] = {
    {"help", help, "Show help"},
    {"?", help, "Show help"},
    {"hexdump", hexdump, "Dumps memory. Usage: dumpmem [hexaddr] [len]"},
    {"spitst", spitst, "Writes data to SPI and dumps answer received"},
    {"nrftst", nrftst, "Reads nRF24L01+ status register"},
    {"nrftst2", nrftst2, "Tests transmission of a packet and other functions"},
    {"nrftst3", nrftst3, "Tests reception of a packet"},
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