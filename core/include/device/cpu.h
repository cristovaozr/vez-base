/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_CPU_H_
#define CORE_INCLUDE_DEVICE_CPU_H_

#include <stdint.h>

struct cpu {
    int32_t (*get_uuid)(const struct cpu * const cpu, void * const uuid, uint32_t size);
    int32_t (*get_rtc_timestamp)(const struct cpu * const cpu, uint32_t * const timestamp);
    int32_t (*get_clock_in_hz)(const struct cpu * const cpu, uint32_t * const clock);
    int32_t (*reset)(const struct cpu * const cpu);
};

/* API Definition */

/**
 * @brief Gets a UUID for the processor. Size should be large enough to contain all data
 *
 * @param cpu CPU object
 * @param uuid [out] UUID value
 * @param size Size of uuid memory space
 * @return int32_t E_SUCCESS on success
 */
extern int32_t cpu_get_uuid(const struct cpu * const cpu, void * const uuid, uint32_t size);

/**
 * @brief Gets a timestamp (in secods) from the RTC
 *
 * @param cpu CPU object
 * @param timestamp [out] Number of seconds
 * @return int32_t E_SUCCESS on success
 */
extern int32_t cpu_get_rtc_timestamp(const struct cpu * const cpu, uint32_t * const timestamp);

/**
 * @brief Gets the clock of the CPU in Hertz
 *
 * @param cpu CPU object
 * @param clock [out] Clock in Hz
 * @return int32_t E_SUCCESS on success
 */
extern int32_t cpu_get_clock_in_hz(const struct cpu * const cpu, uint32_t * const clock);

/**
 * @brief Resets the CPU
 *
 * @param cpu CPU object
 * @return int32_t E_SUCCESS on success.
 */
extern int32_t cpu_reset(const struct cpu * const cpu);

#endif // CORE_INCLUDE_DEVICE_CPU_H_