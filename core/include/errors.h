/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_ERRORS_H_
#define CORE_INCLUDE_ERRORS_H_

#include <stdint.h>

/**
 * @brief Contains definitions of errors
 */

enum errors {
    /***** Generic device error *****/
    E_NOTINIT = INT32_MIN,  /** Did not init device */
    E_POLLOPINVALID,        /** Selected poll_op is not valid */
    E_TIMEOUT,              /** Timeout event */

    /***** PWM Related errors *****/
    E_INVALID_DUTY_CYCLE,   /** Duty cycle does not comply */
    E_INVALID_PWM_FREQ,     /** PWM frequency is invalid */
    E_PWM_FREQ_TOO_HIGH,    /** PWM frequency for sound generation is above 4kHz */
    E_PWM_FREQ_TOO_LOW,     /** PWM frequency for sound generation is below 275Hz */

    /***** Generic error codes *****/
    E_SUCCESS = 0,          /** Success */
};

#endif // CORE_INCLUDE_ERRORS_H_