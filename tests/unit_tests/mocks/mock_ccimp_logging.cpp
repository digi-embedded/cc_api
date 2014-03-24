#include "mocks.h"
#include "mock_ccimp_logging.h"

#define MOCK_LOGGING_ENABLED 1    /* Mock disabled. Do printf normally */

void Mock_ccimp_logging_printf_create(void)
{
    return;
}

void Mock_ccimp_logging_printf_destroy(void)
{
    mock("ccimp_hal_logging_vprintf").checkExpectations();
}

void Mock_ccimp_hal_logging_vprintf_expect(debug_t const debug, char const * const buffer)
{
    /* If we are calling expectations, then override default implementation */
    mock("ccimp_hal_logging_vprintf").setData("behavior", MOCK_LOGGING_ENABLED);

    if (strcmp(buffer, CCIMP_LOGGING_PRINTF_DOESNT_EXPECT_A_CALL) != 0)
    {
        mock("ccimp_hal_logging_vprintf").expectOneCall("ccimp_hal_logging_vprintf")
                    .withParameter("debug", debug).withParameter("buffer", buffer);
    }
}

extern "C" {
#if (defined CCIMP_DEBUG_ENABLED)
#include "CppUTestExt/MockSupport_c.h"
#include "ccapi_definitions.h"

void ccimp_hal_logging_vprintf(debug_t const debug, char const * const format, va_list args)
{
    uint8_t behavior;

    behavior = mock_scope_c("ccimp_hal_logging_vprintf")->getData("behavior").value.intValue;
    if (behavior == MOCK_LOGGING_ENABLED)
    {
        char buffer[500];
        vsnprintf(buffer, sizeof(buffer), format, args);   

        mock_scope_c("ccimp_hal_logging_vprintf")->actualCall("ccimp_hal_logging_vprintf")
              ->withIntParameters("debug", debug)->withStringParameters("buffer", buffer);
    }
    else
    {
        /* TODO: This will change when we have proper zones */
        if (!strncmp(format, "FATAL: ",7))
            assert_buffer = (char *)&format[7];
            
        /* ccimp_hal_logging_vprintf_real(debug, format, args); */
    }
    return;
}
#endif

#if (defined CCIMP_DEBUG_ENABLED)
ccimp_status_t ccimp_hal_halt(void)
{
    /* We don't want real implementation */
    /* return ccimp_hal_halt_real(); */
    return CCIMP_STATUS_OK;
}
#endif

/* TODO: Move to mock_ccimp_reset.cpp */
ccimp_status_t ccimp_hal_reset(void)
{
    return ccimp_hal_reset_real();
}
}
