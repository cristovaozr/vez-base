/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef INC_ILI9328_LCD_ADDR_H_
#define INC_ILI9328_LCD_ADDR_H_

#include <stdint.h>

#define ILI9328_LCD_REG (*((volatile uint16_t *)0x6d000000))
#define ILI9328_LCD_RAM (*((volatile uint16_t *)0x6d010000))

#endif // INC_ILI9328_LCD_ADDR_H_