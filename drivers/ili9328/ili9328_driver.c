/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#include "ili9328_driver.h"
#include "ili9328_driver_regs.h"
#include "ili9328_lcd_addr.h"

#include "include/errors.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>
#include <stddef.h>

#define ILI9328_MAX_X   320
#define ILI9328_MAX_Y   240

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

static uint16_t ili9328_read_reg(uint8_t reg);

static void ili9328_write_reg(uint8_t reg, uint16_t val);

static void ili9328_delay(uint8_t delay);

struct ili9328_startup_sequence {
    uint8_t reg;
    uint8_t delay_ms;
    uint16_t value;
};

static const struct ili9328_startup_sequence startup[50] = {
    {0xe5, 0, 0x78f0},
    {ILI9328_REG_DRIVER_OUTPUT_CTRL_1,          0, 0x0100},
    {ILI9328_REG_LCD_DRIVING_CTRL,              0, 0x0400},
    {ILI9328_REG_ENTRY_MODE,                    0, 0x0018}, // FIXME: (cristovaozr) Melhorar notação dos bits aqui
    {ILI9328_REG_RESIZE_CTRL,                   0, 0x0000},
    {ILI9328_REG_DISPLAY_CTRL_2,                0, 0x0207},
    {ILI9328_REG_DISPLAY_CTRL_3,                0, 0x0000},
    {ILI9328_REG_DISPLAY_CTRL_4,                0, 0x0000},
    {ILI9328_REG_RGB_DISPLAY_INTERFACE_CTRL_1,  0, 0x0000},
    {ILI9328_REG_FRAME_MARKER_POS,              0, 0x0000},
    {ILI9328_REG_RGB_DISPLAY_INTERFACE_CTRL_2,  0, 0x0000},
    /* Power-on sequence */
    {ILI9328_REG_POWER_CTRL_1,                  0, 0x0000},
    {ILI9328_REG_POWER_CTRL_2,                  0, 0x0007},
    {ILI9328_REG_POWER_CTRL_3,                  0, 0x0000},
    {ILI9328_REG_POWER_CTRL_4,                  0, 0x0000},
    {ILI9328_REG_DISPLAY_CTRL_1,                200, 0x0001},
    /* Discharge capacitor power voltage */
    {ILI9328_REG_POWER_CTRL_1,                  0, 0x1090},
    {ILI9328_REG_POWER_CTRL_2,                  50, 0x0227},
    {ILI9328_REG_POWER_CTRL_3,                  50, 0x001f},
    {ILI9328_REG_POWER_CTRL_4,                  0, 0x1500},
    {ILI9328_REG_POWER_CTRL_7,                  0, 0x0027},

    {ILI9328_REG_FRAME_RATE_COLOR_CTRL,         50, 0x000d},
    {ILI9328_REG_HORIZONTAL_GRAM_ADDRESS_SET,   0, 0x0000},
    {ILI9328_REG_VERTICAL_GRAM_ADDRESS_SET,     0, 0x0000},
    /* Gamma adjust curve -- copied! Dunno a thing */
    {ILI9328_REG_GAMMA_CTRL_1,                  0, 0x0000},
    {ILI9328_REG_GAMMA_CTRL_2,                  0, 0x0707},
    {ILI9328_REG_GAMMA_CTRL_3,                  0, 0x0307},
    {ILI9328_REG_GAMMA_CTRL_4,                  0, 0x0200},
    {ILI9328_REG_GAMMA_CTRL_5,                  0, 0x0008},
    {ILI9328_REG_GAMMA_CTRL_6,                  0, 0x0004},
    {ILI9328_REG_GAMMA_CTRL_7,                  0, 0x0000},
    {ILI9328_REG_GAMMA_CTRL_8,                  0, 0x0707},
    {ILI9328_REG_GAMMA_CTRL_9,                  0, 0x0002},
    {ILI9328_REG_GAMMA_CTRL_10,                 0, 0x1d04},
    /* Configure GRAM area */
    {ILI9328_REG_HORIZONTAL_ADDRESS_START,      0, 0x0000},
    {ILI9328_REG_HORIZONTAL_ADDRESS_END,        0, 0x00ef},
    {ILI9328_REG_VERTICAL_ADDRESS_START,        0, 0x0000},
    {ILI9328_REG_VERTICAL_ADDRESS_END,          0, 0x013f},
    {ILI9328_REG_DRIVER_OUTPUT_CTRL_2,          0, 0xa700},
    {ILI9328_REG_BASE_IMAGE_DISPLAY_CTRL,       0, 0x0001},
    {ILI9328_REG_VERTICAL_SCROLL_CTRL,          0, 0x0000},
    /* Partial display control */
    {ILI9328_REG_PARTIAL_IMAGE_1_DISPLAY_POS,   0, 0x0000},
    {ILI9328_REG_PARTIAL_IMAGE_1_START_LINE,    0, 0x0000},
    {ILI9328_REG_PARTIAL_IMAGE_1_END_LINE,      0, 0x0000},
    {ILI9328_REG_PARTIAL_IMAGE_2_DISPLAY_POS,   0, 0x0000},
    {ILI9328_REG_PARTIAL_IMAGE_2_START_LINE,    0, 0x0000},
    {ILI9328_REG_PARTIAL_IMAGE_2_END_LINE,      0, 0x0000},
    /* Panel control */
    {ILI9328_REG_PANEL_INTERFACE_CTRL_1,        0, 0x0010},
    {ILI9328_REG_PANEL_INTERFACE_CTRL_2,        0, 0x0600},
    {ILI9328_REG_DISPLAY_CTRL_1,                0, 0x0133}
};

int32_t ili9328_init(void)
{
    int32_t ret = E_SUCCESS;

    uint16_t lcd_id = ili9328_read_reg(0x00);
    if (lcd_id != 0x9328) {
        ret = E_INVALID_HARDWARE;
        goto exit;
    }

    for (int i = 0; i < ARRAY_SIZE(startup); i++) {
        ili9328_write_reg(startup[i].reg, startup[i].value);
        if (startup[i].delay_ms) ili9328_delay(startup[i].delay_ms);
    }
    ILI9328_LCD_REG = ILI9328_REG_WRITE_DATA_GRAM;

    exit:
    return ret;
}

void ili9328_clear_screen(uint16_t bg_color)
{
    const struct ili9328_point home = {0, 0};
    ili9328_set_cursor(&home);
    ILI9328_LCD_REG = ILI9328_REG_WRITE_DATA_GRAM;
    for (int i = 0; i < ILI9328_MAX_X * ILI9328_MAX_Y; i++) {
        ILI9328_LCD_RAM = bg_color;
    }
}

int32_t ili9328_set_cursor(const struct ili9328_point *point)
{
    int32_t ret = 0;

    if (point->x_pos > ILI9328_MAX_X || point->y_pos > ILI9328_MAX_Y) {
        ret = -1;
        goto exit;
    }

    // Since the dislpay is rotated 270 degrees the following construction is done
    ili9328_write_reg(ILI9328_REG_HORIZONTAL_GRAM_ADDRESS_SET, point->y_pos);
    ili9328_write_reg(ILI9328_REG_VERTICAL_GRAM_ADDRESS_SET, ILI9328_MAX_X - 1 - point->x_pos);

    ILI9328_LCD_REG = ILI9328_REG_WRITE_DATA_GRAM;

    exit:
    return ret;
}

int32_t ili9328_set_gram_window(const struct ili9328_window *window)
{
    int32_t ret = E_SUCCESS;

    uint16_t x_pos_st = window->point_st.x_pos;
    uint16_t y_pos_st = window->point_st.y_pos;
    uint16_t x_pos_end = window->point_end.x_pos;
    uint16_t y_pos_end = window->point_end.y_pos;

    if (x_pos_st > x_pos_end || y_pos_st > y_pos_end || x_pos_st > ILI9328_MAX_X || x_pos_end > ILI9328_MAX_X ||
        y_pos_st > ILI9328_MAX_Y || y_pos_end > ILI9328_MAX_Y) {

        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    // Since the display is rotated 270 degrees the following construction is done
    ili9328_write_reg(ILI9328_REG_HORIZONTAL_ADDRESS_START, y_pos_st);
    ili9328_write_reg(ILI9328_REG_VERTICAL_ADDRESS_START, ILI9328_MAX_X - x_pos_end);

    ili9328_write_reg(ILI9328_REG_HORIZONTAL_ADDRESS_END, y_pos_end);
    ili9328_write_reg(ILI9328_REG_VERTICAL_ADDRESS_END, ILI9328_MAX_X - x_pos_st);

    ILI9328_LCD_REG = ILI9328_REG_WRITE_DATA_GRAM;

    exit:
    return ret;
}

int32_t ili9328_clear_window(const struct ili9328_window *window)
{
    int32_t ret = E_SUCCESS;

    if (window == NULL) {
        ret = E_INVALID_PARAMETER;
        goto exit;
    }

    if ((ret = ili9328_set_cursor(&window->point_st)) < 0) {
        goto exit;
    }

    int window_size = (window->point_end.x_pos - window->point_st.x_pos + 1) * (window->point_end.y_pos - window->point_st.y_pos + 1);
    for (int i = 0; i < window_size; i++, ILI9328_LCD_RAM = window->window_color);

    exit:
    return ret;
}

/**
 * @brief Reads a register
 *
 * @param reg Register as defined by ILI9328_REG_* values
 * @return uint16_t Register value
 */
static uint16_t ili9328_read_reg(uint8_t reg)
{
    ILI9328_LCD_REG = reg;
    return ILI9328_LCD_RAM;
}

/**
 * @brief Writes a register
 *
 * @param reg Register as defined by ILI9328_REG_* values
 * @param val Register value;
 */
static void ili9328_write_reg(uint8_t reg, uint16_t val)
{
    ILI9328_LCD_REG = reg;
    ILI9328_LCD_RAM = val;
}

/**
 * @brief Delays some miliseconds
 *
 * @param delay Delay in miliseconds
 */
static void ili9328_delay(uint8_t delay)
{
    vTaskDelay(delay);
}