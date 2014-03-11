/*
 * ccimp_debug.c
 *
 *  Created on: Mar 7, 2014
 *      Author: hbujanda
 */

#include "custom/custom_connector_config.h"

#ifdef UNIT_TEST
#define ccimp_debug_printf       ccimp_debug_printf_real
#endif

/******************** LINUX IMPLEMENTATION ********************/

#if (defined CCAPI_DEBUG)
#include <stdio.h>
#include <stdlib.h>

void ccimp_debug_printf(char const * const message)
{
    printf("%s", message);
}
#else
 /* to avoid ISO C forbids an empty translation unit compiler error */
typedef int dummy;
#endif