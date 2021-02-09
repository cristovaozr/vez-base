#include "include/device/cpu.h"

#include "include/errors.h"

#include <stdint.h>

int32_t cpu_get_uuid(const struct cpu * const cpu, void * const uuid, uint32_t size)
{
    return cpu->get_uuid(cpu, uuid, size);
}

int32_t cpu_get_rtc_timestamp(const struct cpu * const cpu, uint32_t * const timestamp)
{
    return cpu->get_rtc_timestamp(cpu, timestamp);
}

int32_t cpu_get_clock_in_hz(const struct cpu * const cpu, uint32_t * const clock)
{
    return cpu->get_clock_in_hz(cpu, clock);
}

int32_t cpu_reset(const struct cpu * const cpu)
{
    return cpu->reset(cpu);
}