/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2021
 * Please see LICENCE file to information regarding licensing
 */

#include "uda1380_driver.h"

#include "include/device/i2c.h"
#include "include/device/transaction.h"
#include "include/device/i2s.h"

#include "include/errors.h"
#include "ulibc/include/utils.h"
#include "ulibc/include/log.h"

#define UDA1380_WRITE_ADDRESS     0x18

#define UDA1380_REG_EVALCLK       0x00
#define UDA1380_REG_I2S           0x01
#define UDA1380_REG_PWRCTRL       0x02
#define UDA1380_REG_ANAMIX        0x03
#define UDA1380_REG_HEADAMP       0x04
#define UDA1380_REG_MSTRVOL       0x10
#define UDA1380_REG_MIXVOL        0x11
#define UDA1380_REG_MODEBBT       0x12
#define UDA1380_REG_MSTRMUTE      0x13
#define UDA1380_REG_MIXSDO        0x14
#define UDA1380_REG_DECVOL        0x20
#define UDA1380_REG_PGA           0x21
#define UDA1380_REG_ADC           0x22
#define UDA1380_REG_AGC           0x23

#define UDA1380_REG_L3            0x7f
#define UDA1380_REG_HEADPHONE     0x18
#define UDA1380_REG_DEC           0x28

/*
    UDA1380

    Default values
        Register 0x00: 0000:0101:0000:0010 (0x0502)
        Register 0x01: 0000:0000:0000:0000 (0x0000)
        Register 0x02: 0000:0000:0000:0000 (0x0000)
        Register 0x03: 0011:1111:0011:1111 (0x3f3f)

        Register 0x10: 0000:0000:0000:0000 (0x0000)
        Register 0x11: 1111:1111:0000:0000 (0xff00)
        Register 0x12: 0000:0000:0000:0000 (0x0000)
        Register 0x13: 0100:1000:0000:0000 (0x4800)
        Register 0x14: 0000:0000:0000:0000 (0x0000)

        Register 0x20: 0000:0000:0000:0000 (0x0000)
        Register 0x21: 1000:0000:0000:0000 (0x8000)
        Register 0x22: 0000:0000:0000:0010 (0x0002)
        Register 0x23: 0000:0000:0000:0000 (0x0000)

        Soft reset register: 0x7f

    Configuration for UDA1380:
        Register 0x00: 0000:0111:0000:0010 (0x0712) [Liga o DAC, DAC clock é o SYSCLK]
        Register 0x01: 0000:0000:0100:0000 (0x0040) [Input é I2S, saída passando pelo analog mixer, Output é I2S slave]
        Register 0x02: 0010:0100:1100:0000 (0x24c0) [Headphone ON, DAC on, BIAS off, AVC enabled e on]
        Register 0x03: 0000:0000:0000:0000 (0x0000) [Analog Mixer volume máximo]

        Register 0x13: 0000:0000:0000:0000
*/

static const uint8_t uda1380_startup_sequence[5][3] = {
    {UDA1380_REG_EVALCLK,           0x07, 0x12},
    {UDA1380_REG_I2S,               0x00, 0x40},
    {UDA1380_REG_PWRCTRL,           0x24, 0x0c},
    {UDA1380_REG_ANAMIX,            0x00, 0x00},
    {UDA1380_REG_MSTRMUTE,          0x00, 0x00}
};

// static const uint8_t uda1380_startup_sequence[14][3] = {
//     {UDA1380_REG_PWRCTRL,     0xA5, 0xDF}, /** Enable all power for now */
//     {UDA1380_REG_EVALCLK,     0x07, 0x00}, /** CODEC ADC and DAC clock from WSPLL, all clocks enabled. Configured for 8kHz */
//     {UDA1380_REG_I2S,         0x00, 0x00}, /** I2S bus data I/O formats, use digital mixer for output BCKO is slave */
//     {UDA1380_REG_ANAMIX,      0x00, 0x00}, /** Full mixer analog input gain */
//     {UDA1380_REG_HEADAMP,     0x02, 0x02}, /** Enable headphone short circuit protection */
//     {UDA1380_REG_MSTRVOL,     0x00, 0x00}, /** Full master volume */
//     {UDA1380_REG_MIXVOL,      0x00, 0x00}, /** Enable full mixer volume on both channels */
//     {UDA1380_REG_MODEBBT,     0x00, 0x00}, /** Bass and treble boost set to flat */
//     {UDA1380_REG_MSTRMUTE,    0x00, 0x00}, /** Disable mute and de-emphasis */
//     {UDA1380_REG_MIXSDO,      0x00, 0x00}, /** Mixer off, other settings off */

//     {UDA1380_REG_DECVOL,      0x00, 0x00}, /** ADC decimator volume to max */
//     {UDA1380_REG_PGA,         0x00, 0x00}, /** No PGA mute, full gain */
//     {UDA1380_REG_ADC,         0x0f, 0x02}, /** Select line in and MIC, max MIC gain */
//     {UDA1380_REG_AGC,         0x00, 0x00}, /** AGC */
// };

int32_t uda1380_init(const struct i2c_device *i2c)
{
    int32_t ret;

    for(int i = 0; i < ARRAY_SIZE(uda1380_startup_sequence); i++) {
        struct i2c_transaction transaction = {
            .i2c_device_addr = UDA1380_WRITE_ADDRESS,
            .i2c_device_reg = uda1380_startup_sequence[i][0],
            .transaction_size = 2,
            .write_data = &uda1380_startup_sequence[i][1]
        };

        ret = i2c_write(i2c, &transaction, 0);
        if (ret < 0) goto exit;
    }

    // for (int i = 0; i < ARRAY_SIZE(uda1380_startup_sequence); i++) {
    //     uint8_t values[2];
    //     struct i2c_transaction transaction = {
    //         .i2c_device_addr = UDA1380_WRITE_ADDRESS,
    //         .i2c_device_addr= uda1380_startup_sequence[i][0],
    //         .transaction_size = 2,
    //         .read_data = values
    //     };

    //     ret = i2c_read(i2c, &transaction, 0);
    //     if (ret < 0) goto exit;
    //     DBG("uda1380", "Register %.2x=[%.2x %.2x]", uda1380_startup_sequence[i][0], values[0], values[1]);
    // }

    exit:
    return ret;
}

void uda1380_write_blocking(const struct i2s_device *i2s, uint16_t l_ch, uint16_t r_ch)
{
    i2s_write(i2s, l_ch, r_ch);
}