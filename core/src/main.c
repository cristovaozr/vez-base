/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "include/hw_init.h"
#include "FreeRTOS.h"
#include "task.h"

// TODO: remove this cr*p from here!
extern void MX_FREERTOS_Init(void);

/**
 * @brief Main entry point to the firmware
 *
 * @return int Doesn't return. Kept like this to preserve compatibility
 */
int main(void)
{
    // Configuration that should be run before hardware initialization, if any
    hw_init_early_config();
    // Actual hardware initialization
    hw_init();
    // Configuration that shoud be run after hardware initialization, if any
    hw_init_late_config();

    /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init(); 
    /* Start scheduler */
    vTaskStartScheduler();

    while(1);
}
