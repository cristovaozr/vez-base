/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/gpio.h"

#include <stdint.h>

void gpio_write(const struct gpio_device * const gpio, int32_t value)
{
    gpio->ops->gpio_write_op(gpio, value);
}

int32_t gpio_read(const struct gpio_device  * const gpio)
{
    return gpio->ops->gpio_read_op(gpio);
}

void gpio_toggle(const struct gpio_device * const gpio)
{
    gpio->ops->gpio_toggle_op(gpio);
}
