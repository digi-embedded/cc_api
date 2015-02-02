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

#if (defined CCIMP_UDP_TRANSPORT_ENABLED || defined CCIMP_SMS_TRANSPORT_ENABLED)

#if (defined CONNECTOR_SM_CLI)

#include <stdio.h>

void ccapi_cli_thread(void * const argument)
{
    ccapi_data_t * const ccapi_data = argument;

    /* ccapi_data is corrupted, it's likely the implementer made it wrong passing argument to the new thread */
    ASSERT_MSG_GOTO(ccapi_data != NULL, done);

    ccapi_data->thread.cli->status = CCAPI_THREAD_RUNNING;
    while (ccapi_data->thread.cli->status == CCAPI_THREAD_RUNNING)
    {
        ccapi_svc_cli_t * const svc_cli = ccapi_data->service.cli.svc_cli;
        if (svc_cli != NULL)
        {
            switch (svc_cli->cli_thread_status)
            {
                case CCAPI_CLI_THREAD_IDLE:
                case CCAPI_CLI_THREAD_REQUEST_CB_READY:
                {
                    break;
                }
                case CCAPI_CLI_THREAD_REQUEST_CB_QUEUED:
                {
                    ASSERT_MSG_GOTO(ccapi_data->service.cli.user_callback.request != NULL, done);

                    /* Pass data to the user and get possible response from user */ 
                    ccapi_data->service.cli.user_callback.request(svc_cli->transport, 
                                                                       svc_cli->request_string_info.string,
                                                                       svc_cli->response_required ? (char const * *)&svc_cli->response_string_info.string : NULL);
   
                    /* Check if ccfsm has called status callback cancelling the session while we were waiting for the user */
                    if (svc_cli->cli_thread_status == CCAPI_CLI_THREAD_REQUEST_CB_QUEUED)
                    {
                        svc_cli->cli_thread_status = CCAPI_CLI_THREAD_REQUEST_CB_PROCESSED;
                    }
                    ccapi_data->service.cli.svc_cli = NULL;
                    break;
                }
                case CCAPI_CLI_THREAD_REQUEST_CB_PROCESSED:
                case CCAPI_CLI_THREAD_FREE:
                {
                    break;
                }
            }
        }

        ccimp_os_yield();
    }
    ASSERT_MSG_GOTO(ccapi_data->thread.cli->status == CCAPI_THREAD_REQUEST_STOP, done);

    ccapi_data->thread.cli->status = CCAPI_THREAD_NOT_STARTED;
done:
    return;
}

static ccapi_bool_t valid_cli_malloc(void * * ptr, size_t size, ccapi_cli_error_t * const error)
{
    ccapi_bool_t success;
 
    *ptr = ccapi_malloc(size);

    success = CCAPI_BOOL(*ptr != NULL);
    
    if (!success)
    {
        ccapi_logging_line("valid_cli_malloc: error ccimp_os_realloc for %d bytes", size);
        *error = CCAPI_CLI_ERROR_INSUFFICIENT_MEMORY;
    }

    return success;
}

static connector_callback_status_t ccapi_process_cli_request(connector_sm_cli_request_t * const cli_request_ptr, ccapi_data_t * const ccapi_data)
{
    ccapi_svc_cli_t * svc_cli = NULL;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(cli_request_ptr->buffer != NULL && cli_request_ptr->bytes_used, done);

    if (cli_request_ptr->user_context == NULL)
    {
        if (!valid_cli_malloc((void**)&svc_cli, sizeof *svc_cli, &svc_cli->cli_error))
        {
            /* We didn't manage to create a user_context. ccfsm will call response and status callbacks without it */
            goto done;
        }

        cli_request_ptr->user_context = svc_cli;

        svc_cli->transport = cli_request_ptr->transport;
        svc_cli->cli_thread_status = CCAPI_CLI_THREAD_IDLE;
        svc_cli->request_string_info.string = NULL;
        svc_cli->request_string_info.length = 0;
        svc_cli->response_string_info.string = NULL;
        svc_cli->response_string_info.length = 0;
        svc_cli->response_handled_internally = CCAPI_FALSE;
        svc_cli->response_processing.string = NULL;
        svc_cli->response_processing.length = 0;
        svc_cli->response_required = CCAPI_BOOL(cli_request_ptr->response_required);
        svc_cli->more_data = CCAPI_BOOL(cli_request_ptr->more_data);
        svc_cli->cli_error = CCAPI_CLI_ERROR_NONE;
    }
    else
    {
        svc_cli = cli_request_ptr->user_context;

        svc_cli->more_data = CCAPI_BOOL(cli_request_ptr->more_data);
    }

    if (!ccapi_data->config.cli_supported)
    {
        svc_cli->cli_error = CCAPI_CLI_ERROR_NO_CLI_SUPPORT;
        goto done;
    }

    switch (svc_cli->cli_thread_status)
    {
        case CCAPI_CLI_THREAD_IDLE:
        {
            ccapi_logging_line("ccapi_process_cli_request. cli_thread_status=CCAPI_CLI_THREAD_IDLE");

            {
                ccimp_os_realloc_t ccimp_realloc_data;

                ccimp_realloc_data.new_size = svc_cli->request_string_info.length + cli_request_ptr->bytes_used; /* buffer is already null terminated */

                ccimp_realloc_data.old_size = svc_cli->request_string_info.length;
                ccimp_realloc_data.ptr = svc_cli->request_string_info.string;
                if (ccimp_os_realloc(&ccimp_realloc_data) != CCIMP_STATUS_OK)
                {
                    ccapi_logging_line("ccapi_process_cli_request: error ccimp_os_realloc for %d bytes", ccimp_realloc_data.new_size);

                    svc_cli->cli_error = CCAPI_CLI_ERROR_INSUFFICIENT_MEMORY;
                    svc_cli->cli_thread_status = CCAPI_CLI_THREAD_FREE;
                    goto done;
                }
                svc_cli->request_string_info.string = ccimp_realloc_data.ptr;
 
                {
                    uint8_t * const dest_addr = (uint8_t *)svc_cli->request_string_info.string + svc_cli->request_string_info.length;
                    memcpy(dest_addr, cli_request_ptr->buffer, cli_request_ptr->bytes_used);
                }
                svc_cli->request_string_info.length += cli_request_ptr->bytes_used;
            }

            if (!cli_request_ptr->more_data)
            {
                svc_cli->cli_thread_status = CCAPI_CLI_THREAD_REQUEST_CB_READY;

                ccapi_logging_line("ccapi_process_cli_request. cli_thread_status=CCAPI_CLI_THREAD_REQUEST_CB_READY");

                connector_status = connector_callback_busy;
            }
            else
            {
                connector_status = connector_callback_continue;
            }

            break;
        }
        case CCAPI_CLI_THREAD_REQUEST_CB_READY:
        {
            if (ccapi_data->service.cli.svc_cli == NULL)
            {
                svc_cli->cli_thread_status = CCAPI_CLI_THREAD_REQUEST_CB_QUEUED;

                ccapi_data->service.cli.svc_cli = svc_cli;

                ccapi_logging_line("ccapi_process_cli_request. cli_thread_status=CCAPI_CLI_THREAD_REQUEST_CB_READY->CCAPI_CLI_THREAD_REQUEST_CB_QUEUED");
            }
            else
            {
                ccimp_os_yield();
            }

            connector_status = connector_callback_busy;
            break;
        }
        case CCAPI_CLI_THREAD_REQUEST_CB_QUEUED:
        {
            ccimp_os_yield();

            connector_status = connector_callback_busy;
            break;
        }
        case CCAPI_CLI_THREAD_REQUEST_CB_PROCESSED:
        {
            ccapi_logging_line("ccapi_process_cli_request. cli_thread_status=CCAPI_CLI_THREAD_REQUEST_CB_PROCESSED");
            ccapi_free(svc_cli->request_string_info.string);

            if (svc_cli->response_required && svc_cli->response_string_info.string != NULL)
            {
                svc_cli->response_string_info.length = strlen(svc_cli->response_string_info.string);

                if (svc_cli->response_string_info.length != 0)
                {
                    svc_cli->response_string_info.length++; /* Add the null terminator */

                    memcpy(&svc_cli->response_processing, &svc_cli->response_string_info, sizeof svc_cli->response_processing);
                }
            }

            svc_cli->cli_thread_status = CCAPI_CLI_THREAD_FREE;

            connector_status = connector_callback_continue;

            break;
        }
        case CCAPI_CLI_THREAD_FREE:
            ASSERT(svc_cli->cli_thread_status != CCAPI_CLI_THREAD_FREE);
            break;
    }

done:
    return connector_status;
}

static void fill_internal_error(ccapi_svc_cli_t * svc_cli)
{
#define ERROR_MESSAGE "CCAPI Error %d (%s)"

        char const * cli_error_str = NULL;
        size_t cli_error_str_len = 0;

        switch (svc_cli->cli_error)
        {
#define ENUM_TO_CASE_ERR(name)  case name:  cli_error_str = #name; cli_error_str_len = sizeof #name - 1; break
            ENUM_TO_CASE_ERR(CCAPI_CLI_ERROR_NO_CLI_SUPPORT);
            ENUM_TO_CASE_ERR(CCAPI_CLI_ERROR_INSUFFICIENT_MEMORY);
#undef ENUM_TO_CASE_ERR

            case CCAPI_CLI_ERROR_NONE:
            case CCAPI_CLI_ERROR_STATUS_CANCEL:
            case CCAPI_CLI_ERROR_STATUS_ERROR:
            {
                static char const this_cli_error_str[] = "Unexpected error";
                cli_error_str = (char *)this_cli_error_str;
                cli_error_str_len = sizeof this_cli_error_str - 1;
                break;
            }
        }

        cli_error_str_len += sizeof ERROR_MESSAGE;

        if (!valid_cli_malloc((void**)&svc_cli->response_string_info.string, cli_error_str_len, &svc_cli->cli_error))
        {
              return;
        }
        svc_cli->response_string_info.length = sprintf(svc_cli->response_string_info.string, ERROR_MESSAGE, 
                                                                        svc_cli->cli_error, cli_error_str);

        ccapi_logging_line("fill_internal_error: Providing response in buffer at %p: %s", 
                                    svc_cli->response_string_info.string, (char*)svc_cli->response_string_info.string);
}

static connector_callback_status_t ccapi_process_cli_response(connector_sm_cli_response_t * const cli_response_ptr)
{
    ccapi_svc_cli_t * const svc_cli = cli_response_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_cli != NULL, done);

    ccapi_logging_line("ccapi_process_cli_response");

    if (!svc_cli->response_required)
    {
        goto done;
    }

    /* We initialize the response buffer for internal errors just once */
    if (svc_cli->cli_error != CCAPI_CLI_ERROR_NONE && !svc_cli->response_handled_internally)
    {
        svc_cli->response_handled_internally = CCAPI_TRUE;

        fill_internal_error(svc_cli);
        memcpy(&svc_cli->response_processing, &svc_cli->response_string_info, sizeof svc_cli->response_string_info);
    }

    {
        size_t const bytes_to_send = svc_cli->response_processing.length > cli_response_ptr->bytes_available ?
                                                 cli_response_ptr->bytes_available : svc_cli->response_processing.length;

        memcpy(cli_response_ptr->buffer, svc_cli->response_processing.string, bytes_to_send);
        svc_cli->response_processing.string = ((char *)svc_cli->response_processing.string) + bytes_to_send;
 
        cli_response_ptr->bytes_used = bytes_to_send;
        svc_cli->response_processing.length -= cli_response_ptr->bytes_used;
        cli_response_ptr->more_data = CCFSM_BOOL(svc_cli->response_processing.length > 0);
    }

    connector_status = connector_callback_continue;

done:
    return connector_status;
}

static connector_callback_status_t ccapi_process_cli_status(connector_sm_cli_status_t const * const status_ptr, ccapi_data_t * const ccapi_data)
{
    ccapi_svc_cli_t * const svc_cli = status_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_cli != NULL, done);

    ccapi_logging_line("ccapi_process_cli_status: ccapi_cli_error= %d,  status: %d", svc_cli->cli_error, status_ptr->status);

    /* Prior reported errors by ccapi have priority over the ones reported by the cloud */
    if (svc_cli->cli_error == CCAPI_CLI_ERROR_NONE)
    {
        switch (status_ptr->status)
        {
            case connector_sm_cli_status_success:
                svc_cli->cli_error = CCAPI_CLI_ERROR_NONE;
                break;
            case connector_sm_cli_status_cancel:
                svc_cli->cli_error = CCAPI_CLI_ERROR_STATUS_CANCEL;
                break;
            case connector_sm_cli_status_error:
                svc_cli->cli_error = CCAPI_CLI_ERROR_STATUS_ERROR;
                break;
        }
    }

    /* Call the user so he can free allocated response memory and handle errors  */
    if (ccapi_data->config.cli_supported && ccapi_data->service.cli.user_callback.finished != NULL)
    {
       ccapi_bool_t const should_user_free_response_buffer = !svc_cli->response_handled_internally && svc_cli->response_required && svc_cli->response_string_info.string != NULL;
       ccapi_data->service.cli.user_callback.finished(should_user_free_response_buffer ? svc_cli->response_string_info.string : NULL, svc_cli->cli_error);
    }

    /* Free resources */
    if (svc_cli->response_handled_internally == CCAPI_TRUE)
    {
        ccapi_logging_line("ccapi_process_cli_status: Freeing response buffer at %p", svc_cli->response_string_info.string);
        ccapi_free(svc_cli->response_string_info.string);
    }
    ccapi_free(svc_cli);

    connector_status = connector_callback_continue;

done:
    return connector_status;
}

static connector_callback_status_t ccapi_process_cli_response_length(connector_sm_cli_response_length_t * const length_ptr)
{
    ccapi_svc_cli_t const * const svc_cli = length_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_cli != NULL, done);

    length_ptr->total_bytes = svc_cli->response_processing.length;

    connector_status = connector_callback_continue;

done:
    return connector_status;
}
#endif /* (defined CONNECTOR_SM_CLI) */

static connector_callback_status_t ccapi_process_ping_response(connector_sm_ping_response_t const * const response_ptr)
{
    ccapi_svc_ping_t * const svc_ping = response_ptr->user_context;
    connector_callback_status_t connector_status = connector_callback_error;

    ASSERT_MSG_GOTO(svc_ping != NULL, done);

    ccapi_logging_line("ccapi_process_ping_response: %d", response_ptr->status);
   
    switch (response_ptr->status)
    {
        case connector_sm_ping_status_success:
        case connector_sm_ping_status_complete:
            svc_ping->response_error = CCAPI_PING_ERROR_NONE;
            break;
        case connector_sm_ping_status_cancel:
            svc_ping->response_error = CCAPI_PING_ERROR_RESPONSE_CANCEL;
            break;
        case connector_sm_ping_status_timeout:
            svc_ping->response_error = CCAPI_PING_ERROR_RESPONSE_TIMEOUT;
            break;
        case connector_sm_ping_status_error:
            svc_ping->response_error = CCAPI_PING_ERROR_RESPONSE_ERROR;
            break;
    }

    ASSERT_MSG_GOTO(svc_ping->ping_lock != NULL, done);
    
    switch (ccapi_lock_release(svc_ping->ping_lock))
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

connector_callback_status_t ccapi_sm_service_handler(connector_request_id_sm_t const sm_service_request, void * const data, ccapi_data_t * const ccapi_data)
{
    connector_callback_status_t connector_status;

    switch (sm_service_request)
    {
#if (defined CONNECTOR_SM_CLI)
        case connector_request_id_sm_cli_request:
        {
            connector_sm_cli_request_t * const cli_request_ptr = data;

            connector_status = ccapi_process_cli_request(cli_request_ptr, ccapi_data);

            break;
        }
        case connector_request_id_sm_cli_response:
        {
            connector_sm_cli_response_t * const cli_response_ptr = data;

            connector_status = ccapi_process_cli_response(cli_response_ptr);

            break;
        }
        case connector_request_id_sm_cli_status:
        {
            connector_sm_cli_status_t const * const status_ptr = data;

            connector_status = ccapi_process_cli_status(status_ptr, ccapi_data);

            break;
        }
        case connector_request_id_sm_cli_response_length:
        {
            connector_sm_cli_response_length_t * const length_ptr = data;
            
            connector_status = ccapi_process_cli_response_length(length_ptr);

            break;
        }
#endif

        case connector_request_id_sm_ping_request:
        {
            connector_sm_receive_ping_request_t * const ping_request = data;

            ccapi_logging_line("ccapi_sm_service_handler: response %s needed", ping_request->response_required ? "is" : "is not");

            connector_status = connector_callback_continue;
            break;
        }
        case connector_request_id_sm_ping_response:
        {
            connector_sm_ping_response_t const * const response_ptr = data;

            connector_status = ccapi_process_ping_response(response_ptr);

            break;
        }
        case connector_request_id_sm_more_data:
        case connector_request_id_sm_opaque_response:
        case connector_request_id_sm_config_request:
        case connector_request_id_sm_request_connect:
            ASSERT_MSG(0);
            break;
    }

    ASSERT_MSG_GOTO(connector_status != connector_callback_unrecognized, done);

done:
    return connector_status;
}
#endif