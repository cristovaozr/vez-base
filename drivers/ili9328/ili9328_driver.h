/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef INC_ILI9328_DRIVER_H_
#define INC_ILI9328_DRIVER_H_

#include <stdint.h>

struct ili9328_point {
    uint16_t x_pos;
    uint16_t y_pos;
};

struct ili9328_window {
    struct ili9328_point point_st;
    struct ili9328_point point_end;
    uint16_t window_color;
};

/**
 * @brief Starts ILI9328 LCD display controller
 *
 * @return int32_t E_SUCCESS on success
 */
extern int32_t ili9328_init(void);

/**
 * @brief Clears ILI9328 LCD
 *
 * @param bg_color A color as defined by ILI9328_* macros
 */
extern void ili9328_clear_screen(uint16_t bg_color);

/**
 * @brief Moves drawing cursor to (x, y) coordinate on screen
 *
 * @param point Point object to move cursor to
 *
 * @return E_SUCCESS on success
 */
extern int32_t ili9328_set_cursor(const struct ili9328_point *point);

/**
 * @brief Sets GRAM window
 *
 * window Window object that defines the window
 *
 * @return int32_t E_SUCCESS on success
 */
extern int32_t ili9328_set_gram_window(const struct ili9328_window *window);

/**
 * @brief Clears window (sets default bg color)
 *
 * @param window Window object
 * @return int32_t E_SUCCESS on success
 */
extern int32_t ili9328_clear_window(const struct ili9328_window *window);

/* Some colors */

/** RGB color space macro */
#define ILI9328_RGB(r, g, b) (uint16_t)((b & 0x1f) << 11 | (g & 0x3f) << 5 | (r & 0x1f))

#define ILI9328_RED_SOLID       ILI9328_RGB(31, 0,  0)  /** Solid RED color */
#define ILI9328_GREEN_SOLID     ILI9328_RGB(0,  63, 0)  /** Solid GREEN color */
#define ILI9328_BLUE_SOLID      ILI9328_RGB(0,  0,  31) /** Solid BLUE color */
#define ILI9328_YELLOW_SOLID    ILI9328_RGB(31, 63, 0)  /** Solid YELLOW color */
#define ILI9328_WHITE_SOLID     ILI9328_RGB(31, 63, 31) /** Solid WHITE color */
#define ILI9328_BLACK_SOLID     ILI9328_RGB(0,  0,  0)  /** Solid BLACK color */

#endif // INC_ILI9328_DRIVER_H_