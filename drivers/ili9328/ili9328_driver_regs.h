/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef DRIVERS_ILI9328_ILI9328_DRIVER_REGS_H_
#define DRIVERS_ILI9328_ILI9328_DRIVER_REGS_H_

#define ILI9328_REG_DRIVER_CODE_READ                0x00
#define ILI9328_REG_LCD_DRIVING_CTRL                0x02
#define ILI9328_REG_ENTRY_MODE                      0x03
#define ILI9328_REG_RESIZE_CTRL                     0x04

#define ILI9328_REG_DISPLAY_CTRL_1                  0x07
#define ILI9328_REG_DISPLAY_CTRL_2                  0x08
#define ILI9328_REG_DISPLAY_CTRL_3                  0x09
#define ILI9328_REG_DISPLAY_CTRL_4                  0x0a

#define ILI9328_REG_RGB_DISPLAY_INTERFACE_CTRL_1    0x0c
#define ILI9328_REG_RGB_DISPLAY_INTERFACE_CTRL_2    0x0f

#define ILI9328_REG_FRAME_MARKER_POS                0x0d

#define ILI9328_REG_POWER_CTRL_1                    0x10
#define ILI9328_REG_POWER_CTRL_2                    0x11
#define ILI9328_REG_POWER_CTRL_3                    0x12
#define ILI9328_REG_POWER_CTRL_4                    0x13
#define ILI9328_REG_POWER_CTRL_7                    0x29

#define ILI9328_REG_HORIZONTAL_GRAM_ADDRESS_SET     0x20
#define ILI9328_REG_VERTICAL_GRAM_ADDRESS_SET       0x21
#define ILI9328_REG_WRITE_DATA_GRAM                 0x22
#define ILI9328_REG_FRAME_RATE_COLOR_CTRL           0x2b

#define ILI9328_REG_GAMMA_CTRL_1                    0x30
#define ILI9328_REG_GAMMA_CTRL_2                    0x31
#define ILI9328_REG_GAMMA_CTRL_3                    0x32
#define ILI9328_REG_GAMMA_CTRL_4                    0x35
#define ILI9328_REG_GAMMA_CTRL_5                    0x36
#define ILI9328_REG_GAMMA_CTRL_6                    0x37
#define ILI9328_REG_GAMMA_CTRL_7                    0x38
#define ILI9328_REG_GAMMA_CTRL_8                    0x39
#define ILI9328_REG_GAMMA_CTRL_9                    0x3c
#define ILI9328_REG_GAMMA_CTRL_10                   0x3d

#define ILI9328_REG_HORIZONTAL_ADDRESS_START        0x50
#define ILI9328_REG_HORIZONTAL_ADDRESS_END          0x51
#define ILI9328_REG_VERTICAL_ADDRESS_START          0x52
#define ILI9328_REG_VERTICAL_ADDRESS_END            0x53

#define ILI9328_REG_DRIVER_OUTPUT_CTRL_1            0x01
#define ILI9328_REG_DRIVER_OUTPUT_CTRL_2            0x60
#define ILI9328_REG_BASE_IMAGE_DISPLAY_CTRL         0x61
#define ILI9328_REG_VERTICAL_SCROLL_CTRL            0x6a

#define ILI9328_REG_PARTIAL_IMAGE_1_DISPLAY_POS     0x80
#define ILI9328_REG_PARTIAL_IMAGE_1_START_LINE      0x81
#define ILI9328_REG_PARTIAL_IMAGE_1_END_LINE        0x82
#define ILI9328_REG_PARTIAL_IMAGE_2_DISPLAY_POS     0x83
#define ILI9328_REG_PARTIAL_IMAGE_2_START_LINE      0x84
#define ILI9328_REG_PARTIAL_IMAGE_2_END_LINE        0x85

#define ILI9328_REG_PANEL_INTERFACE_CTRL_1          0x90
#define ILI9328_REG_PANEL_INTERFACE_CTRL_2          0x92
#define ILI9328_REG_PANEL_INTERFACE_CTRL_3          0x95

#endif // DRIVERS_ILI9328_ILI9328_DRIVER_REGS_H_