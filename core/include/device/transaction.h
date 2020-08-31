/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
 * Please see LICENCE file to information regarding licensing
 */

#ifndef CORE_INCLUDE_DEVICE_TRANSACTION_H_
#define CORE_INCLUDE_DEVICE_TRANSACTION_H_

#include <stdint.h>

/**
 * @brief Defines a transaction for data
 */
struct transaction {
    uint32_t write_size;
    void *write_data;
    uint32_t read_size;
    void *read_data;
};

#endif // CORE_INCLUDE_DEVICE_TRANSACTION_H_
