#include "test_helper_functions.h"

TEST_GROUP(test_ccapi_dp_ccfsm_callbacks)
{
    void setup()
    {
        Mock_create_all();
        th_start_ccapi();
        th_start_tcp_lan_ipv4();
    }

    void teardown()
    {
        Mock_destroy_all();
    }
};

TEST(test_ccapi_dp_ccfsm_callbacks, testCCFSMResponseCallbackErrorMapping)
{
    connector_request_id_t request_id;
    connector_data_point_response_t dp_respose;
    connector_callback_status_t connector_status;
    ccapi_dp_transaction_info_t transaction_info;

    transaction_info.syncr = ccapi_syncr_create();
    dp_respose.hint = NULL;
    dp_respose.transport = connector_transport_tcp;
    dp_respose.user_context = &transaction_info;
    request_id.data_point_request = connector_request_id_data_point_multiple_response;

    dp_respose.response = connector_data_point_response_t::connector_data_point_response_success;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_respose, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_NONE, transaction_info.response_error);

    dp_respose.response = connector_data_point_response_t::connector_data_point_response_cloud_error;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_respose, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_RESPONSE_CLOUD_ERROR, transaction_info.response_error);

    dp_respose.response = connector_data_point_response_t::connector_data_point_response_bad_request;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_respose, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_RESPONSE_BAD_REQUEST, transaction_info.response_error);

    dp_respose.response = connector_data_point_response_t::connector_data_point_response_unavailable;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_respose, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_RESPONSE_UNAVAILABLE, transaction_info.response_error);
}

TEST(test_ccapi_dp_ccfsm_callbacks, testCCFSMStatusCallbackReleaseFailed)
{
    connector_request_id_t request_id;
    connector_data_point_status_t dp_status;
    connector_callback_status_t connector_status;
    ccapi_dp_transaction_info_t transaction_info;

    transaction_info.syncr = ccapi_syncr_create();
    dp_status.transport = connector_transport_tcp;
    dp_status.status = connector_data_point_status_t::connector_data_point_status_complete;
    dp_status.session_error = connector_session_error_none;
    dp_status.user_context = &transaction_info;
    request_id.data_point_request = connector_request_id_data_point_multiple_status;

    Mock_ccimp_os_syncr_release_return(CCIMP_STATUS_ERROR);

    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_error, connector_status);
}

TEST(test_ccapi_dp_ccfsm_callbacks, testCCFSMStatusCallbackErrorMapping)
{
    connector_request_id_t request_id;
    connector_data_point_status_t dp_status;
    connector_callback_status_t connector_status;
    ccapi_dp_transaction_info_t transaction_info;

    transaction_info.syncr = ccapi_syncr_create();

    dp_status.transport = connector_transport_tcp;
    dp_status.session_error = connector_session_error_none;
    dp_status.user_context = &transaction_info;
    request_id.data_point_request = connector_request_id_data_point_multiple_status;

    dp_status.status = connector_data_point_status_t::connector_data_point_status_complete;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_NONE, transaction_info.status);

    dp_status.status = connector_data_point_status_t::connector_data_point_status_cancel;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_STATUS_CANCEL, transaction_info.status);

    dp_status.status = connector_data_point_status_t::connector_data_point_status_invalid_data;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_STATUS_INVALID_DATA, transaction_info.status);

    dp_status.status = connector_data_point_status_t::connector_data_point_status_session_error;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_STATUS_SESSION_ERROR, transaction_info.status);

    dp_status.status = connector_data_point_status_t::connector_data_point_status_timeout;
    connector_status = ccapi_connector_callback(connector_class_id_data_point, request_id, &dp_status, (void *)ccapi_data_single_instance);
    CHECK_EQUAL(connector_callback_continue, connector_status);
    CHECK_EQUAL(CCAPI_DP_ERROR_STATUS_TIMEOUT, transaction_info.status);
}