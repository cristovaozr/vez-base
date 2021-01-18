# Device folder

This folder contais headers with definitions of the API for "devices" of the arch. Each device is a hardware that the arch contains: GPIO, USART, SPI, I2C, etc.

Each device (GPIO, etc.) contain a set of structures that governs how the device should behave. Each function **must** must exist even if it does nothing.

Also the behavior should be consistent so that higher layers can behave equally.

## Devices with API already defined

* GPIO (arch/bluepill, arch/open407z)
* USART (arch/bluepill, arch/open407z)
* SPI (Not using IRQs for now - arch/bluepill, arch/open407z)
* I2C (Not using IRQs for now - arch/bluepill, arch/open407z)
* I2S (Not using IRQs for now - arch/open407z)

## Devices with API missing

* ADC
* PWM