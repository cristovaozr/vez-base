# vez-base
V.E.Z. - Volta à estaca zero - base code.

"Volta à estaca zero" is a portuguese expression that means "back to square one". This project serves as basic building blocks for firmware of my personal kits. I stored them here in hope that they might be useful in the future and to other.

The project contais the following structure:

```
vez-base/
 +--- arch/
 +--- core/
 |     +--- include/
 |     |     `- device/
 |     +--- src/
 |           `- device/
 +--- freertos/
 |     +--- blinky/
 |     `--- shell/
 +--- tasks/
 |
 `--- ulibc
```

## "arch" folder

Contains all plataform-dependent code (Bluepill, Waveshare, etc.). Each folder in `arch` contains its own structure that is available in the documentation. To build a platform for your own project just follow instructions contained in `HOWTO-en.md` or `HOWTO.md` if you can understando portuguese.

## "core" folder

Contains `include` and `src` folders with main source-code and headers. Please note folders `include/device` and `src/device` that contais API for device access.

### "device" folder

This folder contais API for device acccess. There is a `README.md` file explaining how the API works.

## "freertos" folder

FreeRTOS files that **do not** depend on architecture. The architecture-dependent files from FreeRTOS generally are stored in a folder called `portable`.

## "tasks" folder

Contains source-code for the tasks that run on FreeRTOS.

## "ulibc" folder

Contains source-code for a C library that I use in embedded projects. I have been having some issues using functions such as `printf()` in previous projects. As long as I don't find out how to use these functions they are implemented here.

This project depends on `newlib` and links against the `newlib-nano` C library for functions that are not defined here. There is also some logging functions available in `log.h` and `log.c`.
