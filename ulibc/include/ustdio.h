/**
 * @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
 * @version 0.1
 *
 * @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020-2021
 * Please see LICENCE file to information regarding licensing
 */

#ifndef ULIBC_INCLUDE_USTDIO_H_
#define ULIBC_INCLUDE_USTDIO_H_

#include <stdarg.h>

extern int uprintf(const char *fmt, ...);

extern int uvprintf(const char *fmt, va_list ap);

extern int ugetchar(void);

extern int uputchar(int c);

extern int uputs(const char *s);

#endif // ULIBC_INCLUDE_USTDIO_H_
