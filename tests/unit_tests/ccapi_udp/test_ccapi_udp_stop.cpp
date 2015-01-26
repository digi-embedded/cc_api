/*
* Copyright (c) 2014 Etherios, a Division of Digi International, Inc.
* All rights not expressly granted are reserved.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/.
*
* Etherios 11001 Bren Road East, Minnetonka, MN 55343
* =======================================================================
*/

#include "test_helper_functions.h"

TEST_GROUP(test_ccapi_udp_stop)
{
    void setup()
    {
        Mock_create_all();
    }

    void teardown()
    {
        Mock_destroy_all();
    }
};

TEST(test_ccapi_udp_stop, testCCAPINotStarted)
{
    ccapi_udp_stop_error_t udp_stop_error;
    ccapi_udp_stop_t udp_stop = {CCAPI_TRANSPORT_STOP_GRACEFULLY};

    udp_stop_error = ccapi_stop_transport_udp(&udp_stop);
    CHECK_EQUAL(CCAPI_UDP_STOP_ERROR_NOT_STARTED, udp_stop_error);
}

TEST(test_ccapi_udp_stop, testUDPNotStarted)
{
    ccapi_udp_stop_error_t udp_stop_error;
    ccapi_udp_stop_t udp_stop = {CCAPI_TRANSPORT_STOP_GRACEFULLY};

    th_start_ccapi();

    udp_stop_error = ccapi_stop_transport_udp(&udp_stop);
    CHECK_EQUAL(CCAPI_UDP_STOP_ERROR_NOT_STARTED, udp_stop_error);
}

TEST(test_ccapi_udp_stop, testUDPStopOK)
{
    ccapi_udp_stop_error_t udp_stop_error;
    ccapi_udp_stop_t udp_stop = {CCAPI_TRANSPORT_STOP_GRACEFULLY};

    th_start_ccapi();
    th_start_udp();

    connector_initiate_stop_request_t stop_data = {connector_transport_udp, connector_wait_sessions_complete, NULL};
    Mock_connector_initiate_action_expectAndReturn(ccapi_data_single_instance->connector_handle, connector_initiate_transport_stop, &stop_data, connector_success);

    udp_stop_error = ccapi_stop_transport_udp(&udp_stop);
    CHECK_EQUAL(CCAPI_UDP_STOP_ERROR_NONE, udp_stop_error);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_stop, testUDPStopCcfsmError)
{
    ccapi_udp_stop_error_t udp_stop_error;
    ccapi_udp_stop_t udp_stop = {CCAPI_TRANSPORT_STOP_GRACEFULLY};

    th_start_ccapi();
    th_start_udp();

    connector_initiate_stop_request_t stop_data = {connector_transport_udp, connector_wait_sessions_complete, NULL};
    Mock_connector_initiate_action_expectAndReturn(ccapi_data_single_instance->connector_handle, connector_initiate_transport_stop, &stop_data, connector_device_error);

    udp_stop_error = ccapi_stop_transport_udp(&udp_stop);
    CHECK_EQUAL(CCAPI_UDP_STOP_ERROR_CCFSM, udp_stop_error);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_stop, testStartStopUDPMemoryLeaks)
{
    ccapi_start_t start = {0};
    ccapi_start_error_t error;
    ccimp_os_create_thread_info_t expected_create_thread_connector_run;

    connector_handle_t handle = &handle; /* Not-NULL */
    void * const malloc_for_ccapi_data = th_expect_malloc(sizeof(ccapi_data_t), TH_MALLOC_RETURN_NORMAL, true);
    th_expect_malloc(sizeof(TH_DEVICE_TYPE_STRING), TH_MALLOC_RETURN_NORMAL, true);
    th_expect_malloc(sizeof(TH_DEVICE_CLOUD_URL_STRING), TH_MALLOC_RETURN_NORMAL, true);
    th_expect_malloc(sizeof (ccapi_thread_info_t), TH_MALLOC_RETURN_NORMAL, true);

    Mock_connector_init_expectAndReturn(ccapi_connector_callback, handle, ccapi_data_single_instance);

    expected_create_thread_connector_run.argument = malloc_for_ccapi_data;
    expected_create_thread_connector_run.type = CCIMP_THREAD_FSM;
    /* expected_create_thread_connector_run.start */
    Mock_ccimp_os_create_thread_expectAndReturn(&expected_create_thread_connector_run, MOCK_THREAD_ENABLED_NORMAL, CCIMP_STATUS_OK);

    th_fill_start_structure_with_good_parameters(&start);
    error = ccapi_start(&start);
    CHECK(error == CCAPI_START_ERROR_NONE);

    CHECK(start.vendor_id == ccapi_data_single_instance->config.vendor_id);
    CHECK(memcmp(start.device_id, ccapi_data_single_instance->config.device_id, sizeof start.device_id) == 0);
    STRCMP_EQUAL(start.device_type, ccapi_data_single_instance->config.device_type);
    STRCMP_EQUAL(start.device_cloud_url, ccapi_data_single_instance->config.device_cloud_url);
    CHECK(ccapi_data_single_instance->thread.connector_run->status == CCAPI_THREAD_RUNNING);
    CHECK(ccapi_data_single_instance->transport_tcp.info == NULL);
    CHECK(ccapi_data_single_instance->transport_udp.info == NULL);
    CHECK(ccapi_data_single_instance->transport_sms.info == NULL);

    th_expect_malloc(sizeof *ccapi_data_single_instance->transport_udp.info, TH_MALLOC_RETURN_NORMAL, true);
    th_start_udp();

    connector_initiate_stop_request_t stop_request;
    stop_request.transport = connector_transport_udp;
    stop_request.condition = connector_wait_sessions_complete;
    stop_request.user_context = NULL;

    Mock_connector_initiate_action_expectAndReturn(ccapi_data_single_instance->connector_handle, connector_initiate_transport_stop, &stop_request, connector_success);
    Mock_connector_initiate_action_expectAndReturn(ccapi_data_single_instance->connector_handle, connector_initiate_terminate, NULL, connector_success);

    ccapi_stop(CCAPI_STOP_GRACEFULLY);
}
