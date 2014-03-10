/*
 * ccustom_debug.h
 *
 *  Created on: Mar 6, 2014
 *      Author: hbujanda
 */

#ifndef CCIMP_DEBUG_H_
#define CCIMP_DEBUG_H_

#include "custom/custom_connector_config.h"

#if (defined CCAPI_DEBUG)
/**
 * Debug output from Cloud Connector, Writes a formatted string to stdout, expanding the format
 * tags with the value of the argument list arg.  This function behaves exactly as
 * printf except that the variable argument list is passed as a va_list instead of a
 * succession of arguments.
 *
 * In the C library the prototype for vprintf is defined as vprintf(const char *format, va_list ap);
 *
 */
void ccimp_debug_printf(char const * const message);

/**
 *  Verify that the condition is true, otherwise halt the program.
 */
#define ASSERT(cond)        assert(cond)

#else
#define ASSERT(cond)
#endif

#endif /* CCIMP_DEBUG_H_ */
