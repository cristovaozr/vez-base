/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_PWM_H_
#define CORE_INCLUDE_DEVICE_PWM_H_

#include <stdint.h>

struct pwm_operations;

struct pwm_device {
    const struct pwm_operations * const ops;
    const void * const priv;
};

struct pwm_operations {
    int32_t (*pwm_init)(const struct pwm_device * const pwm);

    int32_t (*pwm_set_duty)(const struct pwm_device * const pwm, uint32_t duty);

    int32_t (*pwm_set_frequency)(const struct pwm_device * const pwm, uint32_t freq);
};

extern int32_t pwm_set_duty(const struct pwm_device * const pwm, uint32_t duty);

extern int32_t pwm_set_frequency(const struct pwm_device * const pwm, uint32_t freq);

#endif // CORE_INCLUDE_DEVICE_PWM_H_
