/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_HW_INIT_H_
#define CORE_INCLUDE_HW_INIT_H_

#include <stdint.h>

/**
 * @brief Early configuration of hardware that should be performed before the hardware initialization, if any
 */
extern int32_t hw_init_early_config(void);

/**
 * @brief Hardware initialization routine: clocks, PLL, etc.
 */
extern int32_t hw_init(void);

/**
 * @brief Late configuration of hardware that should be performed before the hardware initialization, if any
 */
extern int32_t hw_init_late_config(void);

#endif // CORE_INCLUDE_HW_INIT_H_