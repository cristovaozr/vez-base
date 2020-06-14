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
    E_NOTINIT = INT32_MIN,  /** Did not init device */
    E_POLLOPINVALID,        /** Selected poll_op is not valid */
    E_TIMEOUT,

    E_SUCCESS = 0,
};

#endif // CORE_INCLUDE_ERRORS_H_