# Setting up an arch for your microcontroller

This document explains how to setup an arch(itecture) folder for your microcontroller (board). Please see `bluepill` for more examples.

# Setting up configuration for the Makefile

A file named `config.mk` should exist for each arch definition. On this file arch-specific information for building the project should be inserted. At least the following variables should be set:

* `ARCH_MCU`
* `ARCH_C_DEFS`
* `ARCH_C_INCLUDES`
* `ARCH_C_SOURCES`
* `ARCH_AS_INCLUDES`
* `ARCH_ASM_SOURCES`
* `ARCH_LDSCRIPT`

# Setting minimal files and functions to build

To be able to build an arch variant the following files should exist:

* An implementation of hardware initialization routines as exported by `${VEZ-BASE}/core/include/hw_init.h` file. Most arches name this file `src/hw_init.c`;

# Exported symbols

There **must** exist a file called `include/exported.h` that contains all objects, structures, etc. that implements any interface from the general API. All objects that appear in `include/exported.h` **must** be marked with a `EXPORTED` tag available at `${VEZ-BASE}/include/utils.h`