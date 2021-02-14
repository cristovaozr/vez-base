/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#include "include/device/pwm.h"

#include <stdint.h>

int32_t pwm_set_duty(const struct pwm_device * const pwm, uint32_t duty)
{
    return pwm->ops->pwm_set_duty(pwm, duty);
}

int32_t pwm_set_frequency(const struct pwm_device * const pwm, uint32_t freq)
{
    return pwm->ops->pwm_set_frequency(pwm, freq);
}
