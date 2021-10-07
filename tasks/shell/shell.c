/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "FreeRTOS.h"
#include "task.h"

#include "components/vez-shell/include/vez-shell.h"

#define TAG "SHELL"

static void shell_task(void *arg)
{
    (void)arg;

    vez_shell_greeter(NULL);

    while (1) {
        const struct vez_shell_entry *cmds = SHELL_COMMAND_ARRAY_ADDR;
        vez_shell_iterate(cmds, SHELL_COMMAND_SIZE);
    }
}

#define SHELL_TASK_SIZE 1024
static StackType_t shell_stack[SHELL_TASK_SIZE];
static StaticTask_t shell_tcb;

void declare_shell_task(void)
{
    xTaskCreateStatic(shell_task, TAG, SHELL_TASK_SIZE, NULL, tskIDLE_PRIORITY, shell_stack, &shell_tcb);
}