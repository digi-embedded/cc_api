#include "mocks.h"

void Mock_ccimp_network_udp_open_create(void)
{
    mock().installComparator("ccimp_network_open_t", ccimp_network_open_t_comparator);
    return;
}

void Mock_ccimp_network_udp_open_destroy(void)
{
    mock("ccimp_network_udp_open").checkExpectations();
}

void Mock_ccimp_network_udp_open_expectAndReturn(ccimp_network_open_t * expect, ccimp_status_t retval)
{
    mock("ccimp_network_udp_open").expectOneCall("ccimp_network_udp_open")
            .withParameterOfType("ccimp_network_open_t", "open_data", expect)
            .andReturnValue(retval);
}

void Mock_ccimp_network_udp_send_create(void)
{
    mock().installComparator("ccimp_network_send_t", ccimp_network_send_t_comparator);

    return;
}

void Mock_ccimp_network_udp_send_destroy(void)
{
    mock("ccimp_network_udp_send").checkExpectations();
}

void Mock_ccimp_network_udp_send_expectAndReturn(ccimp_network_send_t * expect, ccimp_status_t retval)
{
    mock("ccimp_network_udp_send").expectOneCall("ccimp_network_udp_send")
            .withParameterOfType("ccimp_network_send_t", "send_data", expect)
            .andReturnValue(retval);
}

void Mock_ccimp_network_udp_receive_create(void)
{
    mock().installComparator("ccimp_network_receive_t", ccimp_network_receive_t_comparator);

    return;
}

void Mock_ccimp_network_udp_receive_destroy(void)
{
    mock("ccimp_network_udp_receive").checkExpectations();
}

void Mock_ccimp_network_udp_receive_expectAndReturn(ccimp_network_receive_t * expect, ccimp_status_t retval)
{
    mock("ccimp_network_udp_receive").expectOneCall("ccimp_network_udp_receive")
            .withParameterOfType("ccimp_network_receive_t", "receive_data", expect)
            .andReturnValue(retval);
}

void Mock_ccimp_network_udp_close_create(void)
{
    mock().installComparator("ccimp_network_close_t", ccimp_network_close_t_comparator);

    return;
}

void Mock_ccimp_network_udp_close_destroy(void)
{
    mock("ccimp_network_udp_close").checkExpectations();
}

void Mock_ccimp_network_udp_close_expectAndReturn(ccimp_network_close_t * expect, ccimp_status_t retval)
{
    mock("ccimp_network_udp_close").expectOneCall("ccimp_network_udp_close")
            .withParameterOfType("ccimp_network_close_t", "close_data", expect)
            .andReturnValue(retval);
}

extern "C" {
#include "CppUTestExt/MockSupport_c.h"
#include "ccapi_definitions.h"

ccimp_status_t ccimp_network_udp_open(ccimp_network_open_t * const open_data)
{
    mock_scope_c("ccimp_network_udp_open")->actualCall("ccimp_network_udp_open")->withParameterOfType("ccimp_network_open_t", "open_data", (void *)open_data);

    return (ccimp_status_t)mock_scope_c("ccimp_network_udp_open")->returnValue().value.intValue;
}

ccimp_status_t ccimp_network_udp_send(ccimp_network_send_t * const send_data)
{
    mock_scope_c("ccimp_network_udp_send")->actualCall("ccimp_network_udp_send")->withParameterOfType("ccimp_network_send_t", "send_data", (void *)send_data);

    return (ccimp_status_t)mock_scope_c("ccimp_network_udp_send")->returnValue().value.intValue;
}

ccimp_status_t ccimp_network_udp_receive(ccimp_network_receive_t * const receive_data)
{
    mock_scope_c("ccimp_network_udp_receive")->actualCall("ccimp_network_udp_receive")->withParameterOfType("ccimp_network_receive_t", "receive_data", (void *)receive_data);

    return (ccimp_status_t)mock_scope_c("ccimp_network_udp_receive")->returnValue().value.intValue;
}

ccimp_status_t ccimp_network_udp_close(ccimp_network_close_t * const close_data)
{
    mock_scope_c("ccimp_network_udp_close")->actualCall("ccimp_network_udp_close")->withParameterOfType("ccimp_network_close_t", "close_data", (void *)close_data);

    return (ccimp_status_t)mock_scope_c("ccimp_network_udp_close")->returnValue().value.intValue;
}
}
