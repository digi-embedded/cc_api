#include "ccapi/ccapi.h"
#include "ccapi_definitions.h"

#if (defined CCIMP_DEBUG_ENABLED)

/* TODO:
       - Locks to avoid mixing messages
       - categories
*/

void connector_debug_vprintf(ccimp_debug_t const debug, char const * const format, va_list args)
{
    /* TODO: Macro */
    ccimp_debug_vprintf(debug, format, args);
}

#define CALL_DEBUG_VPRINTF(type, format) \
    do \
    { \
        va_list args; \
 \
        va_start(args, (format)); \
        ccimp_debug_vprintf((type), (format), args); \
        va_end(args); \
    } \
    while (0)

void ccapi_debug_line(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_all, format);
}

static void ccapi_debug_line_beg(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_beg, format);
}

static void ccapi_debug_line_mid(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_mid, format);
}

static void ccapi_debug_line_end(char const * const format, ...)
{
    CALL_DEBUG_VPRINTF(debug_end, format);
}

void ccapi_debug_print_buffer(char const * const label, void const * const buffer, size_t const length)
{
    size_t i;
    uint8_t const * const content = buffer;

    ccapi_debug_line_beg("%s:", label);
    for (i = 0; i < length; i++)
    {
        if ((i % 16) == 0)
        {
            ccapi_debug_line_mid("\n");
        }

        ccapi_debug_line_mid(" %02X", content[i]);
    }
    ccapi_debug_line_end("");
}
#else
void ccapi_debug_line(char const * const format, ...)
{
    (void)format;
}
void ccapi_debug_print_buffer(char const * const label, void const * const buffer, size_t const length)
{  
    (void)label;
    (void)buffer;
    (void)length;
}
#endif /* (defined CCIMP_DEBUG_ENABLED) */

