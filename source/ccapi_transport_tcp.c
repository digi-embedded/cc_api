#include "ccapi_definitions.h"
#include "ccapi/ccapi_transport_tcp.h"

static int check_keepalives(ccapi_tcp_info_t const * const tcp_start, ccapi_tcp_start_error_t * const error)
{
    int retval = 0;

    if (tcp_start->keepalives.rx != 0)
    {
        if (tcp_start->keepalives.rx > CCAPI_KEEPALIVES_RX_MAX || tcp_start->keepalives.rx < CCAPI_KEEPALIVES_RX_MIN)
        {
            retval = -1;
            goto done;
        }
    }

    if (tcp_start->keepalives.tx != 0)
    {
        if (tcp_start->keepalives.tx > CCAPI_KEEPALIVES_TX_MAX || tcp_start->keepalives.tx < CCAPI_KEEPALIVES_TX_MIN)
        {
            retval = -1;
            goto done;
        }
    }

    if (tcp_start->keepalives.wait_count != 0)
    {
        if (tcp_start->keepalives.wait_count > CCAPI_KEEPALIVES_WCNT_MAX || tcp_start->keepalives.wait_count < CCAPI_KEEPALIVES_WCNT_MIN)
        {
            retval = -1;
            goto done;
        }
    }

done:
    if (retval != 0)
    {
        ccapi_logging_line("ccxapi_start_transport_tcp: invalid keepalive configuration");
        *error = CCAPI_TCP_START_ERROR_KEEPALIVES;
    }
    else
    {
        *error = CCAPI_TCP_START_ERROR_NONE;
    }

    return retval;
}

static int check_connection(ccapi_tcp_info_t const * const tcp_start, ccapi_tcp_start_error_t * const error)
{
    int retval = 0;

    switch (tcp_start->connection.type)
    {
        case CCAPI_CONNECTION_LAN:
        {
            uint8_t const invalid_mac[MAC_ADDR_LENGTH] = {0};

            if (memcmp(invalid_mac, tcp_start->connection.info.lan.mac_address, sizeof invalid_mac) == 0)
            {
                retval = -1;
                *error = CCAPI_TCP_START_ERROR_INVALID_MAC;
                goto done;
            }

            switch(tcp_start->connection.info.lan.ip.type)
            {
                case CCAPI_IPV4:
                {
                    uint32_t const invalid_ipv4 = 0;

                    if (tcp_start->connection.info.lan.ip.address.ipv4 == invalid_ipv4)
                    {
                        ccapi_logging_line("ccxapi_start_transport_tcp: invalid IPv4");
                        *error = CCAPI_TCP_START_ERROR_IP;
                        retval = -1;
                        goto done;
                    }
                    break;
                }
                case CCAPI_IPV6:
                {
                    uint8_t const invalid_ipv6[IPV6_LENGTH] = {0};

                    if (memcmp(tcp_start->connection.info.lan.ip.address.ipv6, invalid_ipv6, sizeof invalid_ipv6) == 0)
                    {
                        ccapi_logging_line("ccxapi_start_transport_tcp: invalid IPv6");
                        *error = CCAPI_TCP_START_ERROR_IP;
                        retval = -1;
                        goto done;
                    }
                    break;
                }
            }
            break;
        }
        case CCAPI_CONNECTION_WAN:
        {
            if (tcp_start->connection.info.wan.phone_number == NULL)
            {
                ccapi_logging_line("ccxapi_start_transport_tcp: invalid Phone number");
                *error = CCAPI_TCP_START_ERROR_PHONE;
                retval = -1;
                goto done;
            }
        }
            break;
    }
done:
    return retval;
}

static int check_malloc(void * ptr, ccapi_tcp_start_error_t * const error)
{
    if (ptr == NULL)
    {
        *error = CCAPI_TCP_START_ERROR_INSUFFICIENT_MEMORY;
        return -1;
    }
    else
    {
        *error = CCAPI_TCP_START_ERROR_NONE;
        return 0;
    }
}

static void copy_lan_info(ccapi_tcp_info_t * const dest, ccapi_tcp_info_t const * const source)
{
    switch(source->connection.info.lan.ip.type)
    {
        case CCAPI_IPV4:
        {
            dest->connection.info.lan.ip.address.ipv4 = source->connection.info.lan.ip.address.ipv4;
            break;
        }
        case CCAPI_IPV6:
        {
            memcpy(dest->connection.info.lan.ip.address.ipv6, dest->connection.info.lan.ip.address.ipv6, sizeof dest->connection.info.lan.ip.address.ipv6);
            break;
        }
    }
}

static int copy_wan_info(ccapi_tcp_info_t * const dest, ccapi_tcp_info_t const * const source, ccapi_tcp_start_error_t * const error)
{
    int retval = 0;

    if (source->connection.info.wan.phone_number != NULL)
    {
        dest->connection.info.wan.phone_number = ccapi_malloc(strlen(source->connection.info.wan.phone_number) + 1);
        retval = check_malloc(dest->connection.info.wan.phone_number, error);
        if (retval != 0)
        {
            goto done;
        }
        strcpy(dest->connection.info.wan.phone_number, source->connection.info.wan.phone_number);
    }

done:
    return retval;
}

static int copy_ccapi_tcp_info_t_structure(ccapi_tcp_info_t * const dest, ccapi_tcp_info_t const * const source, ccapi_tcp_start_error_t * const error)
{
    int retval = 0;
    *dest = *source; /* Strings and pointers to buffer need to be copied manually to allocated spaces. */

    if (dest->keepalives.rx == 0)
    {
        dest->keepalives.rx = CCAPI_KEEPALIVES_RX_DEFAULT;
    }
    if (dest->keepalives.tx == 0)
    {
        dest->keepalives.tx = CCAPI_KEEPALIVES_TX_DEFAULT;
    }
    if (dest->keepalives.wait_count == 0)
    {
        dest->keepalives.wait_count = CCAPI_KEEPALIVES_WCNT_DEFAULT;
    }

    if (source->connection.password != NULL)
    {
        dest->connection.password = ccapi_malloc(strlen(source->connection.password) + 1);
        retval = check_malloc(dest->connection.password, error);
        if (retval != 0)
        {
            goto done;
        }
        strcpy(dest->connection.password, source->connection.password);
    }

    switch (source->connection.type)
    {
        case CCAPI_CONNECTION_LAN:
        {
            copy_lan_info(dest, source);
            break;
        }
        case CCAPI_CONNECTION_WAN:
        {
            retval = copy_wan_info(dest, source, error);
            break;
        }
    }
done:
    return retval;
}

ccapi_tcp_start_error_t ccxapi_start_transport_tcp(ccapi_handle_t const ccapi_handle, ccapi_tcp_info_t const * const tcp_start)
{
    ccapi_tcp_start_error_t error = CCAPI_TCP_START_ERROR_NONE;
    ccapi_data_t * ccapi_data = (ccapi_data_t *) ccapi_handle;

    if (ccapi_data == NULL || ccapi_data->thread.connector_run->status == CCAPI_THREAD_NOT_STARTED)
    {
        ccapi_logging_line("ccxapi_start_transport_tcp: CCAPI not started");

        error = CCAPI_TCP_START_ERROR_CCAPI_STOPPED;
        goto done;
    }

    if (tcp_start == NULL)
    {
        ccapi_logging_line("ccxapi_start_transport_tcp: invalid argument %p", (void *)tcp_start);
        error = CCAPI_TCP_START_ERROR_NULL_POINTER;
        goto done;
    }

    ccapi_data->transport.tcp = ccapi_malloc(sizeof *ccapi_data->transport.tcp);
    if (check_malloc(ccapi_data->transport.tcp, &error) != 0)
    {
        goto done;
    }

    if (check_keepalives(tcp_start, &error) != 0)
    {
        goto done;
    }

    if (check_connection(tcp_start, &error) != 0)
    {
        goto done;
    }

    if (copy_ccapi_tcp_info_t_structure(ccapi_data->transport.tcp, tcp_start, &error) != 0)
    {
        goto done;
    }

done:
    return error;
}

ccapi_tcp_start_error_t ccapi_start_transport_tcp(ccapi_tcp_info_t const * const tcp_start)
{
    return ccxapi_start_transport_tcp(ccapi_data_single_instance, tcp_start);
}

