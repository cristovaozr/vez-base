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
    E_NOT_INITIALIZED = INT32_MIN,  /** Did not init device */
    E_POLLOP_INVALID,        /** Selected poll_op is not valid */
    E_TIMEOUT,              /** Timeout event */
    E_INVALID_PARAMETER,    /** Invalid parameter */
    E_INVALID_HARDWARE,     /** Invalid hardware detected */
    E_HARDWARE_CONFIG_FAILED, /** Hardware configuration failure */
    E_DEVICE_NOT_FOUND,     /** Requested device not found */

    /***** PWM Related errors *****/
    E_INVALID_DUTY_CYCLE,   /** Duty cycle does not comply */
    E_INVALID_PWM_FREQ,     /** PWM frequency is invalid */
    E_PWM_FREQ_TOO_HIGH,    /** PWM frequency for sound generation is above 4kHz */
    E_PWM_FREQ_TOO_LOW,     /** PWM frequency for sound generation is below 275Hz */

    /***** FIFO errors *****/
    E_TX_QUEUE_FULL,        /** TX queue full */
    E_RX_QUEUE_EMPTY,       /** RX queue empty */

    /***** Generic error codes *****/
    E_UNIMPEMENTED,         /** Function not implemented yet */
    E_SUCCESS = 0,          /** Success */
};

/**
 * @brief Converts an error number to its string equivalent
 *
 * @param error Error code
 * @return A string representation of the error code
 */
extern const char *error_to_str(enum errors error);

#endif // CORE_INCLUDE_ERRORS_H_