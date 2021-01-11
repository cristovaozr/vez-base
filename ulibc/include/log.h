/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef ULIBC_INCLUDE_LOG_H_
#define ULIBC_INCLUDE_LOG_H_

#include <stdint.h>

/**
 * @brief Logging helper functions
 */

enum log_level {
    DEBUG_LVL,  // Green color
    INFO_LVL,   // White color
    WARN_LVL,   // Yellow color
    ERROR_LVL   // Red color
};

/**
 * @brief Logs something to the default output
 * 
 * @param level LogLevel
 * @param fmt 
 * @param ... 
 */
extern void ulog(enum log_level level, const char *tag, const char *fmt, ...);

/**
 * @brief Prints log in hexadecimal format
 * 
 * @param data 
 * @param len 
 */
extern void hex_ulog(const void *data, uint32_t len);

/**
 * @brief Helper marco for DEBUG
 */
#if (RELEASE)
#define DBG(tag, fmt, ...)
#else
#define DBG(tag, fmt, ...) ulog(DEBUG_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for INFO
 */
#if (RELEASE)
#define INFO(tag, fmt, ...)
#else
#define INFO(tag, fmt, ...) ulog(INFO_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for WARN
 */
#if (RELEASE)
#define WARN(tag, fmt, ...)
#else
#define WARN(tag, fmt, ...) ulog(WARN_LVL, tag, fmt, ##__VA_ARGS__)
#endif

/**
 * @brief Helper macro for ERROR
 */
#define ERROR(tag, fmt, ...) ulog(ERROR_LVL, tag, fmt, ##__VA_ARGS__)

/**
 * @brief Helper macro for HexLox
 * 
 */
#if (RELEASE)
#define HEXDUMP(data, len)
#else
#define HEXDUMP(data, len) hex_ulog(data, len)
#endif 

#endif // ULIBC_INCLUDE_LOG_H_
