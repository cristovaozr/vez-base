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

#include "include/errors.h"
#include "ulibc/include/utils.h"

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

static const uint8_t uda1380_startup_sequence[14][3] = {
    {UDA1380_REG_PWRCTRL,     0xA5, 0xDF}, /** Enable all power for now */
    {UDA1380_REG_EVALCLK,     0x07, 0x00}, /** CODEC ADC and DAC clock from WSPLL, all clocks enabled. Configured for 8kHz */
    {UDA1380_REG_I2S,         0x00, 0x00}, /** I2S bus data I/O formats, use digital mixer for output BCKO is slave */
    {UDA1380_REG_ANAMIX,      0x00, 0x00}, /** Full mixer analog input gain */
    {UDA1380_REG_HEADAMP,     0x02, 0x02}, /** Enable headphone short circuit protection */
    {UDA1380_REG_MSTRVOL,     0x00, 0x00}, /** Full master volume */
    {UDA1380_REG_MIXVOL,      0x00, 0x00}, /** Enable full mixer volume on both channels */
    {UDA1380_REG_MODEBBT,     0x00, 0x00}, /** Bass and treble boost set to flat */
    {UDA1380_REG_MSTRMUTE,    0x00, 0x00}, /** Disable mute and de-emphasis */
    {UDA1380_REG_MIXSDO,      0x00, 0x00}, /** Mixer off, other settings off */

    {UDA1380_REG_DECVOL,      0x00, 0x00}, /** ADC decimator volume to max */
    {UDA1380_REG_PGA,         0x00, 0x00}, /** No PGA mute, full gain */
    {UDA1380_REG_ADC,         0x0f, 0x02}, /** Select line in and MIC, max MIC gain */
    {UDA1380_REG_AGC,         0x00, 0x00}, /** AGC */
};

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
        if (ret != E_SUCCESS) goto exit;
    }

    exit:
    return ret;
}

void uda1380_write_blocking(uint16_t l_ch, uint16_t r_ch)
{
    // while (LL_I2S_IsActiveFlag_TXE(SPI2) == 0);
    // LL_I2S_TransmitData16(SPI2, l_ch);
    // while (LL_I2S_IsActiveFlag_TXE(SPI2) == 0);
    // LL_I2S_TransmitData16(SPI2, r_ch);
}