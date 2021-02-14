/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_DEVICE_H_
#define CORE_INCLUDE_DEVICE_DEVICE_H_

#include <stdint.h>

/**
 * @brief API to configure devices from exported.h
 */

/**
 * @brief Function to call each device init in the device_operations structure
 *
 * @param device Pointer to any structure declared in "exported.h"
 * @return 0 if configuration is successful; a negative number on error
 */
extern int32_t device_init(const void * const device);

/**
 * @brief Obtains a device by its name
 *
 * @param dev_name Device name 
 * @return const void* Generic pointer to device
 */
extern const void *device_get_by_name(const char *dev_name);

/** USART used for Shell */
#define DEFAULT_USART   "default_usart"
/** LED used for blinky task */
#define DEFAULT_LED     "led_gpio"
/** Default CPU */
#define DEFAULT_CPU     "cpu"

#endif // CORE_INCLUDE_DEVICE_DEVICE_H_
