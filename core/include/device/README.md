# Device folder

This folder contais headers with definitions of the API for "devices" of the arch. Each device is a hardware that the arch contains: GPIO, USART, SPI, I2C, etc.

Each device (GPIO, etc.) contain a set of structures that governs how the device should behave. Each function **must** must exist even if it does nothing.

Also the behavior should be consistent so that higher layers can behave equally.

## Devices with API already defined

* GPIO
* USART
* SPI (Not using IRQs for now)

## Devices with API missing

* ADC
* I2C