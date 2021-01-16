/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "ulibc/include/ustdio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "include/device/device.h"
#include "include/device/gpio.h"

void blinky(void *arg)
{
    (void)arg;
    const struct gpio_device *gpio = device_get_by_name("led_gpio");
    if (gpio == NULL) {
        uprintf("Could not get GPIO LED device\r\n");
        while (1);
    }

    while (1) {
        gpio_toggle(gpio);
        vTaskDelay(250);
    }
}