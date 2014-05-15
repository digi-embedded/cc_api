#include "test_helper_functions.h"

TEST_GROUP(test_ccapi_udp_close)
{
    void setup()
    {
        Mock_create_all();

        th_start_ccapi();
        th_start_udp();
    }

    void teardown()
    {
        th_stop_ccapi(ccapi_data_single_instance);

        Mock_destroy_all();
    }
};


TEST(test_ccapi_udp_close, testUdpCloseCallbackCloudDisconnected)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_cloud_disconnected, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_TRUE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(CCAPI_UDP_CLOSE_DISCONNECTED, ccapi_udp_close_cb_argument);
    CHECK_EQUAL(connector_true, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}



TEST(test_ccapi_udp_close, testUdpCloseDeviceError)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_device_error, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_TRUE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(CCAPI_UDP_CLOSE_DATA_ERROR, ccapi_udp_close_cb_argument);
    CHECK_EQUAL(connector_false, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseNoCallback)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_cloud_disconnected, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    ccapi_data_single_instance->transport_udp.info->callback.close = NULL;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);
    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(connector_false, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseAbort)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_abort, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(connector_false, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseTerminated)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_device_terminated, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(connector_false, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseStopped)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_device_stopped, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_OK);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_continue, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(connector_false, connector_close_data.reconnect);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseError)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_device_stopped, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_ERROR);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_error, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(CCAPI_TRUE, ccapi_data_single_instance->transport_udp.started);
}

TEST(test_ccapi_udp_close, testUdpCloseBusy)
{
    connector_network_handle_t handle = &handle;
    connector_request_id_t request;
    connector_network_close_t connector_close_data = {handle, connector_close_status_device_stopped, connector_false};
    ccimp_network_close_t ccimp_close_data = {handle};
    connector_callback_status_t status;

    Mock_ccimp_network_udp_close_expectAndReturn(&ccimp_close_data, CCIMP_STATUS_BUSY);

    request.network_request = connector_request_id_network_close;
    status = ccapi_connector_callback(connector_class_id_network_udp, request, &connector_close_data, ccapi_data_single_instance);

    CHECK_EQUAL(connector_callback_busy, status);
    CHECK_EQUAL(CCAPI_FALSE, ccapi_udp_close_cb_called);
    CHECK_EQUAL(CCAPI_TRUE, ccapi_data_single_instance->transport_udp.started);
}