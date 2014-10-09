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

#define CCAPI_CONST_PROTECTION_UNLOCK

#include "ccapi_definitions.h"

#if (defined CCIMP_DATA_SERVICE_ENABLED)
static connector_callback_status_t ccapi_process_send_data_request(connector_data_service_send_data_t * const send_ptr)
{
    connector_callback_status_t status = connector_callback_error;
	
    if (send_ptr != NULL)
    {
        ccapi_svc_send_t * const svc_send = (ccapi_svc_send_t *)send_ptr->user_context;
        size_t bytes_expected_to_read;

        ASSERT_MSG_GOTO(svc_send != NULL, done);
        bytes_expected_to_read = send_ptr->bytes_available > svc_send->bytes_remaining ? svc_send->bytes_remaining : send_ptr->bytes_available;

        if (svc_send->sending_file == CCAPI_FALSE)
        {
            memcpy(send_ptr->buffer, svc_send->next_data, bytes_expected_to_read);
            svc_send->next_data = ((char *)svc_send->next_data) + bytes_expected_to_read;
        }
#if (defined CCIMP_FILE_SYSTEM_SERVICE_ENABLED)
        else
        {
            size_t bytes_read;

            ccimp_status_t ccimp_status = ccapi_read_file(svc_send->ccapi_data, svc_send->file_handler, send_ptr->buffer, bytes_expected_to_read, &bytes_read);
            if (ccimp_status != CCIMP_STATUS_OK)
            {
                svc_send->request_error = CCAPI_SEND_ERROR_ACCESSING_FILE;
                goto done;
            }

            ASSERT_MSG(bytes_expected_to_read == bytes_read);
        }
#endif

        send_ptr->bytes_used = bytes_expected_to_read;
        svc_send->bytes_remaining -= send_ptr->bytes_used;
        send_ptr->more_data = svc_send->bytes_remaining > 0 ? connector_true : connector_false;

        status = connector_callback_continue;
    }
    else
    {
        ccapi_logging_line("ccapi_process_send_data_request: no app data set to send");
    }

done:
    return status;
}

static connector_callback_status_t ccapi_process_send_data_response(connector_data_service_send_response_t const * const resp_ptr)
{
    ccapi_svc_send_t * const svc_send = (ccapi_svc_send_t *)resp_ptr->user_context;

    ASSERT_MSG_GOTO(svc_send != NULL, done);

    /* TODO: we could have a flag in svc_send where to check if user wants a response or not to skip this callback */

    ccapi_logging_line("Received %s response from Device Cloud", resp_ptr->response == connector_data_service_send_response_success ? "success" : "error");

    switch (resp_ptr->response)
    {
        case connector_data_service_send_response_success:
            svc_send->response_error = CCAPI_SEND_ERROR_NONE;
            break;
        case connector_data_service_send_response_bad_request:
            svc_send->response_error = CCAPI_SEND_ERROR_RESPONSE_BAD_REQUEST;
            break;
        case connector_data_service_send_response_unavailable:
            svc_send->response_error = CCAPI_SEND_ERROR_RESPONSE_UNAVAILABLE;
            break;
        case connector_data_service_send_response_cloud_error:
            svc_send->response_error = CCAPI_SEND_ERROR_RESPONSE_CLOUD_ERROR;
            break;
        case connector_data_service_send_response_COUNT:
            ASSERT_MSG_GOTO(resp_ptr->response != connector_data_service_send_response_COUNT, done);
            break;
    }

    if (resp_ptr->hint != NULL)
    {
        ccapi_logging_line("Device Cloud response hint %s", resp_ptr->hint);
        if (svc_send->hint != NULL)
        {
            size_t const max_hint_length = svc_send->hint->length - 1;

            strncpy(svc_send->hint->string, resp_ptr->hint, max_hint_length);
            svc_send->hint->string[max_hint_length] = '\0';
        }
    }

done:
    return connector_callback_continue;
}

static connector_callback_status_t ccapi_process_send_data_status(connector_data_service_status_t const * const status_ptr)
{
    ccapi_svc_send_t * const svc_send = (ccapi_svc_send_t *)status_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_send != NULL, done);

    ccapi_logging_line("ccapi_process_send_data_status: %d", status_ptr->status);
   
    switch (status_ptr->status)
    {
        case connector_data_service_status_complete:
            svc_send->status_error = CCAPI_SEND_ERROR_NONE;
            break;
        case connector_data_service_status_cancel:
            svc_send->status_error = CCAPI_SEND_ERROR_STATUS_CANCEL;
            break;
        case connector_data_service_status_timeout:
            svc_send->status_error = CCAPI_SEND_ERROR_STATUS_TIMEOUT;
            break;
        case connector_data_service_status_session_error:
            svc_send->status_error = CCAPI_SEND_ERROR_STATUS_SESSION_ERROR;
            ccapi_logging_line("ccapi_process_send_data_status: session_error=%d", status_ptr->session_error);
            break;
        case connector_data_service_status_COUNT:
            ASSERT_MSG_GOTO(status_ptr->status != connector_data_service_status_COUNT, done);
            break;
    }

    ASSERT_MSG_GOTO(svc_send->send_syncr != NULL, done);
    
    switch (ccapi_syncr_release(svc_send->send_syncr))
    {
        case CCIMP_STATUS_OK:
            connector_status = connector_callback_continue;
            break;
        case CCIMP_STATUS_BUSY:
        case CCIMP_STATUS_ERROR:
            connector_status = connector_callback_error;
            break;
    }

done:
    return connector_status;
}

static connector_callback_status_t ccapi_process_send_data_length(connector_data_service_length_t * const length_ptr)
{
    ccapi_svc_send_t * const svc_send = (ccapi_svc_send_t *)length_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_send != NULL, done);

    length_ptr->total_bytes = svc_send->bytes_remaining;

    ccapi_logging_line("ccapi_process_send_data_length: %d", length_ptr->total_bytes);
   
    connector_status = connector_callback_continue;

done:
    return connector_status;
}

static ccapi_bool_t valid_receive_malloc(void * * ptr, size_t size, ccapi_receive_error_t * const error)
{
    ccapi_bool_t success;
 
    *ptr = ccapi_malloc(size);

    success = *ptr == NULL ? CCAPI_FALSE : CCAPI_TRUE;
    
    if (!success)
    {
        *error = CCAPI_RECEIVE_ERROR_INSUFFICIENT_MEMORY;
    }

    return success;
}

static connector_callback_status_t ccapi_process_device_request_target(connector_data_service_receive_target_t * const target_ptr, ccapi_data_t * const ccapi_data)
{
    ccapi_svc_receive_t * svc_receive = NULL;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(target_ptr->target != NULL, done);

    ccapi_logging_line("ccapi_process_device_request_target for target = '%s'", target_ptr->target);

    ASSERT_MSG_GOTO(target_ptr->user_context == NULL, done);

    {
        if (!valid_receive_malloc((void**)&svc_receive, sizeof *svc_receive, &svc_receive->receive_error))
        {
            /* We didn't manage to create a user_context. ccfsm will call response and status callbacks without it */
            goto done;
        }

        target_ptr->user_context = svc_receive;

        svc_receive->target = NULL;
        svc_receive->user_callbacks.data_cb = ccapi_data->service.receive.user_callbacks.data_cb;
        svc_receive->user_callbacks.status_cb = ccapi_data->service.receive.user_callbacks.status_cb;
        svc_receive->max_request_size = CCAPI_RECEIVE_NO_LIMIT;
        svc_receive->request_buffer_info.buffer = NULL;
        svc_receive->request_buffer_info.length = 0;
        svc_receive->response_buffer_info.buffer = NULL;
        svc_receive->response_buffer_info.length = 0;
        svc_receive->response_handled_internally = CCAPI_FALSE;
        svc_receive->response_processing.buffer = NULL;
        svc_receive->response_processing.length = 0;
        svc_receive->receive_error = CCAPI_RECEIVE_ERROR_NONE;

        svc_receive->response_required = target_ptr->response_required == connector_true ? CCAPI_TRUE : CCAPI_FALSE;

        /* CCAPI_RECEIVE_ERROR_CCAPI_STOPPED is not handled here. We assume that if we get a request
           means that ccapi is running. That error will be used in add_receive_target()
         */

        {
            const size_t target_size = strlen(target_ptr->target) + 1;

            if (!valid_receive_malloc((void**)&svc_receive->target, target_size, &svc_receive->receive_error))
            {
                goto done;
            }
            memcpy(svc_receive->target, target_ptr->target, target_size);
        }

        if (!ccapi_data->config.receive_supported)
        {
            svc_receive->receive_error = CCAPI_RECEIVE_ERROR_NO_RECEIVE_SUPPORT;
            goto done;
        }

        /* Check if it's a registered target */
        {
            ccapi_receive_target_t const * const added_target = *get_pointer_to_target_entry(ccapi_data, target_ptr->target);

            if (added_target != NULL)
            {
                svc_receive->max_request_size = added_target->max_request_size;
                svc_receive->user_callbacks.data_cb = added_target->user_callbacks.data_cb;
                svc_receive->user_callbacks.status_cb = added_target->user_callbacks.status_cb;

                connector_status = connector_callback_continue;
                goto done;
            }
        }

        if (svc_receive->user_callbacks.data_cb == NULL)
        {
            svc_receive->receive_error = CCAPI_RECEIVE_ERROR_INVALID_DATA_CB;
            goto done;
        }

        /* Ask user if accepts target */
        {
            ccapi_bool_t user_accepts;

            if (ccapi_data->service.receive.user_callbacks.accept_cb != NULL)
            {
                user_accepts = ccapi_data->service.receive.user_callbacks.accept_cb(svc_receive->target, target_ptr->transport);
            }
            else
            {
                /* User didn't provide an accept callback. We accept it always */
                user_accepts = CCAPI_TRUE;
            }

            if (user_accepts == CCAPI_TRUE)
            {
                connector_status = connector_callback_continue;
            }
            else
            {
                svc_receive->receive_error = CCAPI_RECEIVE_ERROR_USER_REFUSED_TARGET;
            }
        }
    }

done:
    return connector_status;
}

static connector_callback_status_t ccapi_process_device_request_data(connector_data_service_receive_data_t * const data_ptr, ccapi_data_t * const ccapi_data)
{
    ccapi_svc_receive_t * const svc_receive = (ccapi_svc_receive_t *)data_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_receive != NULL, done);

    ccapi_logging_line("ccapi_process_device_request_data for target = '%s'", svc_receive->target);

    if (!ccapi_data->config.receive_supported)
    {
        svc_receive->receive_error = CCAPI_RECEIVE_ERROR_NO_RECEIVE_SUPPORT;
        goto done;
    }

    {
        ccimp_os_realloc_t ccimp_realloc_data;

        ccimp_realloc_data.new_size = svc_receive->request_buffer_info.length + data_ptr->bytes_used;

        if (svc_receive->max_request_size != CCAPI_RECEIVE_NO_LIMIT && ccimp_realloc_data.new_size > svc_receive->max_request_size)
        {
            ccapi_logging_line("ccapi_process_device_request_data: request excess max_request_size (%d) for this target", svc_receive->max_request_size);

            svc_receive->receive_error = CCAPI_RECEIVE_ERROR_REQUEST_TOO_BIG;
            goto done;
        }

        ccimp_realloc_data.old_size = svc_receive->request_buffer_info.length;
        ccimp_realloc_data.ptr = svc_receive->request_buffer_info.buffer;
        if (ccimp_os_realloc(&ccimp_realloc_data) != CCIMP_STATUS_OK)
        {
            ccapi_logging_line("ccapi_process_device_request_data: error ccimp_os_realloc for %d bytes", ccimp_realloc_data.new_size);

            svc_receive->receive_error = CCAPI_RECEIVE_ERROR_INSUFFICIENT_MEMORY;
            goto done;
        }
        svc_receive->request_buffer_info.buffer = ccimp_realloc_data.ptr;
 
        {
            uint8_t * const dest_addr = (uint8_t *)svc_receive->request_buffer_info.buffer + svc_receive->request_buffer_info.length;
            memcpy(dest_addr, data_ptr->buffer, data_ptr->bytes_used);
        }
        svc_receive->request_buffer_info.length += data_ptr->bytes_used;
    }

    if (data_ptr->more_data == connector_false)
    {
        ASSERT_MSG_GOTO(svc_receive->user_callbacks.data_cb != NULL, done);

        /* Pass data to the user and get possible response from user */ 
        {
            svc_receive->user_callbacks.data_cb(svc_receive->target, data_ptr->transport, 
                                                               &svc_receive->request_buffer_info, 
                                                               svc_receive->response_required ? &svc_receive->response_buffer_info : NULL);

            ccapi_free(svc_receive->request_buffer_info.buffer);

            if (svc_receive->response_required)
            {
                memcpy(&svc_receive->response_processing, &svc_receive->response_buffer_info, sizeof svc_receive->response_buffer_info);
            }
        }
    }

    connector_status = connector_callback_continue;

done:
    return connector_status;
}

static void fill_internal_error(ccapi_svc_receive_t * svc_receive)
{
#define ERROR_MESSAGE "CCAPI Error %d (%s) while handling target '%s'"

        char const * receive_error_str = NULL;
        size_t receive_error_str_len = 0;

        switch (svc_receive->receive_error)
        {
#define ENUM_TO_CASE_ERR(name)  case name:  receive_error_str = #name; receive_error_str_len = sizeof #name - 1; break
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_CCAPI_NOT_RUNNING);
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_NO_RECEIVE_SUPPORT);
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_INSUFFICIENT_MEMORY);
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_INVALID_DATA_CB);
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_USER_REFUSED_TARGET);
            ENUM_TO_CASE_ERR(CCAPI_RECEIVE_ERROR_REQUEST_TOO_BIG);
#undef ENUM_TO_CASE_ERR

            case CCAPI_RECEIVE_ERROR_NONE:
            case CCAPI_RECEIVE_ERROR_INVALID_TARGET:
            case CCAPI_RECEIVE_ERROR_TARGET_NOT_ADDED:
            case CCAPI_RECEIVE_ERROR_TARGET_ALREADY_ADDED:
            case CCAPI_RECEIVE_ERROR_SYNCR_FAILED:
            case CCAPI_RECEIVE_ERROR_STATUS_CANCEL:
            case CCAPI_RECEIVE_ERROR_STATUS_TIMEOUT:
            case CCAPI_RECEIVE_ERROR_STATUS_SESSION_ERROR:
            {
                static char const this_receive_error_str[] = "Unexpected error";
                receive_error_str = (char *)this_receive_error_str;
                receive_error_str_len = sizeof this_receive_error_str - 1;
                break;
            }
        }

        receive_error_str_len += sizeof ERROR_MESSAGE + strlen(svc_receive->target);

        if (!valid_receive_malloc(&svc_receive->response_buffer_info.buffer, receive_error_str_len, &svc_receive->receive_error))
        {
              return;
        }
        svc_receive->response_buffer_info.length = sprintf(svc_receive->response_buffer_info.buffer, ERROR_MESSAGE, 
                                                                        svc_receive->receive_error, receive_error_str, svc_receive->target);

        ccapi_logging_line("fill_internal_error: Providing response in buffer at %p: %s", 
                                    svc_receive->response_buffer_info.buffer, (char*)svc_receive->response_buffer_info.buffer);
}

static connector_callback_status_t ccapi_process_device_request_response(connector_data_service_receive_reply_data_t * const reply_ptr)
{
    ccapi_svc_receive_t * const svc_receive = (ccapi_svc_receive_t *)reply_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_receive != NULL, done);

    ccapi_logging_line("ccapi_process_device_request_response for target = '%s'", svc_receive->target);

    if (!svc_receive->response_required)
    {
        goto done;
    }

    /* We initialize the response buffer for internal errors just once */
    if (svc_receive->receive_error != CCAPI_RECEIVE_ERROR_NONE && svc_receive->response_handled_internally == CCAPI_FALSE)
    {
        svc_receive->response_handled_internally = CCAPI_TRUE;

        fill_internal_error(svc_receive);
        memcpy(&svc_receive->response_processing, &svc_receive->response_buffer_info, sizeof svc_receive->response_buffer_info);
    }

    {
        size_t const bytes_to_send = svc_receive->response_processing.length > reply_ptr->bytes_available ?
                                                 reply_ptr->bytes_available : svc_receive->response_processing.length;

        memcpy(reply_ptr->buffer, svc_receive->response_processing.buffer, bytes_to_send);
        svc_receive->response_processing.buffer = ((char *)svc_receive->response_processing.buffer) + bytes_to_send;
 
        reply_ptr->bytes_used = bytes_to_send;
        svc_receive->response_processing.length -= reply_ptr->bytes_used;
        reply_ptr->more_data = svc_receive->response_processing.length > 0 ? connector_true : connector_false;
    }

    connector_status = connector_callback_continue;

done:
    return connector_status;
}

static connector_callback_status_t ccapi_process_device_request_status(connector_data_service_status_t const * const status_ptr, ccapi_data_t * const ccapi_data)
{
    ccapi_svc_receive_t * const svc_receive = (ccapi_svc_receive_t *)status_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_receive != NULL, done);

    ccapi_logging_line("ccapi_process_device_request_status for target = '%s'", svc_receive->target);
    ccapi_logging_line("ccapi_process_device_request_status: ccapi_receive_error= %d,  status: %d", svc_receive->receive_error, status_ptr->status);

    /* Prior reported errors by ccapi have priority over the ones reported by the cloud */
    if (svc_receive->receive_error == CCAPI_RECEIVE_ERROR_NONE)
    {
        switch (status_ptr->status)
        {
            case connector_data_service_status_complete:
                svc_receive->receive_error = CCAPI_RECEIVE_ERROR_NONE;
                break;
            case connector_data_service_status_cancel:
                svc_receive->receive_error = CCAPI_RECEIVE_ERROR_STATUS_CANCEL;
                break;
            case connector_data_service_status_timeout:
                svc_receive->receive_error = CCAPI_RECEIVE_ERROR_STATUS_TIMEOUT;
                break;
            case connector_data_service_status_session_error:
                svc_receive->receive_error = CCAPI_RECEIVE_ERROR_STATUS_SESSION_ERROR;
                ccapi_logging_line("ccapi_process_device_request_status: session_error=%d", status_ptr->session_error);
                break;
            case connector_data_service_status_COUNT:
                ASSERT_MSG_GOTO(status_ptr->status != connector_data_service_status_COUNT, done);
                break;
        }
    }

    /* Call the user so he can free allocated response memory and handle errors  */
    if (ccapi_data->config.receive_supported && svc_receive->user_callbacks.status_cb != NULL)
    {
       const ccapi_bool_t should_user_free_response_buffer = !svc_receive->response_handled_internally && svc_receive->response_required && svc_receive->response_buffer_info.buffer != NULL;
       svc_receive->user_callbacks.status_cb(svc_receive->target, status_ptr->transport, 
                           should_user_free_response_buffer ? &svc_receive->response_buffer_info : NULL, 
                           svc_receive->receive_error);
    }

    /* Free resources */
    if (svc_receive->target != NULL)
    {
        ccapi_free(svc_receive->target);
    }
    if (svc_receive->response_handled_internally == CCAPI_TRUE)
    {
        ccapi_logging_line("ccapi_process_device_request_status: Freeing response buffer at %p", svc_receive->response_buffer_info.buffer);
        ccapi_free(svc_receive->response_buffer_info.buffer);
    }
    ccapi_free(svc_receive);

    connector_status = connector_callback_continue;

done:
    return connector_status;
}


static connector_callback_status_t ccapi_process_device_request_length(connector_data_service_length_t * const length_ptr)
{
    ccapi_svc_receive_t const * const svc_receive = (ccapi_svc_receive_t *)length_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_receive != NULL, done);

    length_ptr->total_bytes = svc_receive->response_processing.length;

    connector_status = connector_callback_continue;

done:
    return connector_status;
}

connector_callback_status_t ccapi_data_service_handler(connector_request_id_data_service_t const data_service_request, void * const data, ccapi_data_t * const ccapi_data)
{
    connector_callback_status_t connector_status;

    switch (data_service_request)
    {
        case connector_request_id_data_service_send_data:
        {
            connector_data_service_send_data_t * const send_ptr = data;

            connector_status = ccapi_process_send_data_request(send_ptr);

            break;
        }
        case connector_request_id_data_service_send_response:
        {
            connector_data_service_send_response_t const * const resp_ptr = data;

            connector_status = ccapi_process_send_data_response(resp_ptr);

            break;
        }
        case connector_request_id_data_service_send_status:
        {
            connector_data_service_status_t const * const status_ptr = data;
            
            connector_status = ccapi_process_send_data_status(status_ptr);

            break;
        }
        case connector_request_id_data_service_send_length:
        {
            connector_data_service_length_t * const length_ptr = data;
            
            connector_status = ccapi_process_send_data_length(length_ptr);

            break;
        }
        case connector_request_id_data_service_receive_target:
        {
            connector_data_service_receive_target_t * const target_ptr = data;

            connector_status = ccapi_process_device_request_target(target_ptr, ccapi_data);

            break;
        }           
        case connector_request_id_data_service_receive_data:
        {
            connector_data_service_receive_data_t * const data_ptr = data;

            connector_status = ccapi_process_device_request_data(data_ptr, ccapi_data);

            break;
        }
        case connector_request_id_data_service_receive_reply_data:
        {
            connector_data_service_receive_reply_data_t * const reply_ptr = data;

            connector_status = ccapi_process_device_request_response(reply_ptr);

            break;
        }
        case connector_request_id_data_service_receive_status:
        {
            connector_data_service_status_t const * const status_ptr = data;

            connector_status = ccapi_process_device_request_status(status_ptr, ccapi_data);

            break;
        }
        case connector_request_id_data_service_receive_reply_length:
        {
            connector_data_service_length_t * const length_ptr = data;
            
            connector_status = ccapi_process_device_request_length(length_ptr);

            break;
        }
    }

    ASSERT_MSG_GOTO(connector_status != connector_callback_unrecognized, done);

done:
    return connector_status;
}
#endif