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
#include "include/device/gpio.h"

void blinky(void *arg)
{
    (void)arg;
    while(1) {
        gpio_toggle(&led_gpio);
        vTaskDelay(250);
    }
}