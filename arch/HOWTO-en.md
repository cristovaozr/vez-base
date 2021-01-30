# Setting up a platform for your MCU

This document explains how to configure a platform folder (arch) for your microcontroller. The project [vez-arch-bluepill](https://github.com/cristovaozr/vez-arch-bluepill) is an example on how to create a platform.

# Creating a configuration for the Makefile

O main `Makefile` (the one that belongs on the root folder) depends on the inclusion of a file `config.mk`. This file contains the configuration (symbols and other stuff) needed to compile the platform. It is necessary to define the following symbols:

|       Símbolo      | Significado |
|---|---|
| `ARCH_MCU` | Must define the CPU conforming to GCC flags: -mcpu, -mthumb, etc. |
| `ARCH_C_DEFS` | Must contain all `#define` flags for C/C++ (e.g.: `-DCLOCK=8000000`) |
| `ARCH_C_INCLUDES` | Must contain all paths for the platform `#include` for C/C++ |
| `ARCH_C_SOURCES` | Must contain all C/C++ source files for compiling |
| `ARCH_AS_INCLUDES` | Must contain all paths for the platform `#include` for assembly |
| `ARCH_ASM_SOURCES` | Must contain all assembly source files for compiling |
| `ARCH_LDSCRIPT` | Must contain the path for the linker file |

# Minimum API for compilation

To compile a platform the following files must exist:

| File | Explanation |
|---|---|
| Hardware initialization | Implements the API defined by `${VEZ-BASE}/core/include/hw_init.h`. Normally the platforms calls this file `hw_init.c` |
| Files for defices implementation | Implements the API defined by `${VEZ-BASE}/core/include/device/device.h` and implements the function `const void * device_get_by_name(const char *dev_name)`

# Minimum hardware that should be available

The following devices should be defined
* `DEFAULT_USART`
* `DEFAULT_LED`

```c
#include "include/device/device.h"

// A GPIO (normally attributed to a LED) for the blinky task
const struct gpio_device *led = device_get_by_name(DEFAULT_LED);

// A USART for the shell
const struct usart_device *usart = device_get_by_name(DEFAULT_USART);
```

# Pending stuff

The following is still pending to detach the base project to all platforms:

* Create an "empty" platform that allows compilation of the project without the need to import a platform for the project