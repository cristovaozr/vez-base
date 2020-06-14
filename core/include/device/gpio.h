/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_GPIO_H_
#define CORE_INCLUDE_DEVICE_GPIO_H_

#include <stdint.h>

struct gpio_operations;

/**
 * @brief Defines a object that is a GPIO
 */
struct gpio_device {
    const struct gpio_operations * const ops;
    /**
     * @brief Private data from the object. Normally contains arch-dependent stuff
     */
    const void * const priv;
};

/**
 * @brief Possible operations on GPIO objects.
 * Every *_operaitions structure should have the first funptr a init()-like function:
 * it should only receive a "struct *_device * const" and nothing more
 */
struct gpio_operations {
    /**
     * @brief Initializes the GPIO object
     */
    int32_t (*gpio_init)(const struct gpio_device * const gpio);

    /**
     * @brief Writes a value to the GPIO pin. Zero means LOW and non-zero means HIGH
     */
    void    (*gpio_write_op)(const struct gpio_device * const gpio, int32_t value);

    /**
     * @brief Reads the value of the GPIO pin. Zero means LOW and non-zero means HIGH
     */
    int32_t (*gpio_read_op)(const struct gpio_device * const gpio);

    /**
     * @brief Toggles the value of the GPIO pin.
     */
    void    (*gpio_toggle_op)(const struct gpio_device * const gpio);
};

/*
 * API Definition
 */

/**
 * @brief Writes a value to GPIO
 *
 * @param gpio Object that represents a GPIO
 * @param value 0 writes LOW on GPIO, everyting else writes HIGH
 */
extern void gpio_write(const struct gpio_device * const gpio, int32_t value);

/**
 * @brief Reads the value of a GPIO
 *
 * @param gpio Object that represents a GPIO
 * @return int32_t returns 0 for LOW, 1 for HIGH
 */
extern int32_t gpio_read(const struct gpio_device  * const gpio);

/**
 * @brief Toggles the state of the GPIO
 *
 * @param gpio Object that represents a GPIO
 */
extern void gpio_toggle(const struct gpio_device * const gpio);

#endif // CORE_INCLUDE_DEVICE_GPIO_H_
