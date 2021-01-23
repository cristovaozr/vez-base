/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#include "include/errors.h"

#define ERRSTR(x) case x: return #x

const char *error_to_str(enum errors error)
{
    switch (error) {
        ERRSTR(E_NOTINIT);
        ERRSTR(E_POLLOPINVALID);
        ERRSTR(E_TIMEOUT);
        ERRSTR(E_INVALID_PARAMETER);
        ERRSTR(E_INVALID_HARDWARE);
        ERRSTR(E_HARDWARE_CONFIG_FAILED);

        /***** PWM Related errors *****/
        ERRSTR(E_INVALID_DUTY_CYCLE);
        ERRSTR(E_INVALID_PWM_FREQ);
        ERRSTR(E_PWM_FREQ_TOO_HIGH);
        ERRSTR(E_PWM_FREQ_TOO_LOW);

        /***** FIFO errors *****/
        ERRSTR(E_TX_QUEUE_FULL);
        ERRSTR(E_RX_QUEUE_EMPTY);

        /***** Generic error codes *****/
        ERRSTR(E_UNIMPEMENTED);
        ERRSTR(E_SUCCESS);
        default: return "UNKNOWN";
    }
}