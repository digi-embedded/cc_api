#include "ccapi_definitions.h"
#include "ccimp/ccimp_network.h"

#ifdef UNIT_TEST
#define ccimp_network_tcp_open       ccimp_network_tcp_open_real
#define ccimp_network_tcp_send       ccimp_network_tcp_send_real
#define ccimp_network_tcp_receive    ccimp_network_tcp_receive_real
#define ccimp_network_tcp_close      ccimp_network_tcp_close_real
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "connector_api.h"
#include "dns_helper.h"

/* Global structure of connected interface */
static struct sockaddr_in interface_addr;

ccimp_status_t ccimp_network_tcp_close(ccimp_network_close_t * const data)
{
    ccimp_status_t status = CCIMP_STATUS_OK;
    int * const fd = data->handle;

    if (close(*fd) < 0)
    {
        printf("network_tcp_close: close() failed, fd %d, errno %d\n", *fd, errno);
    }
    else
        printf("network_tcp_close: fd %d\n", *fd);

    *fd = -1;

    return status;
}



ccimp_status_t ccimp_network_tcp_receive(ccimp_network_receive_t * const data)
{
    ccimp_status_t status = CCIMP_STATUS_OK;
    int * const fd = data->handle;

    int ccode = read(*fd, data->buffer, data->bytes_available);
    if (ccode > 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    if (ccode == 0)
    {
        /* EOF on input: the connection was closed. */
        printf("network_receive: EOF on socket\n");
        errno = ECONNRESET;
        status = CCIMP_STATUS_ABORT;
    }
    else
    {
        int const err = errno;
        /* An error of some sort occurred: handle it appropriately. */
        if (err == EAGAIN)
        {
            status = CCIMP_STATUS_BUSY;
        }
        else
        {
            printf("network_receive: recv() failed, errno %d\n", err);
            /* if not timeout (no data) return an error */
            dns_cache_invalidate();
            status = CCIMP_STATUS_ABORT;
        }
    }
    return status;
}

ccimp_status_t ccimp_network_tcp_send(ccimp_network_send_t * const data)
{
    ccimp_status_t status = CCIMP_STATUS_OK;
    int * const fd = data->handle;

    int ccode = write(*fd, data->buffer, data->bytes_available);
    if (ccode >= 0)
    {
        data->bytes_used = (size_t)ccode;
    }
    else
    {
        int const err = errno;
        if (err == EAGAIN)
        {
            status = CCIMP_STATUS_BUSY;
        }
        else
        {
            status = CCIMP_STATUS_ABORT;
            printf("app_network_tcp_send: send() failed, errno %d\n", err);
            dns_cache_invalidate();
        }
    }

    return status;
}


static int app_tcp_create_socket(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd >= 0)
    {
        int enabled = 1;

        if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enabled, sizeof(enabled)) < 0)
        {
            printf("open_socket: setsockopt SO_KEEPALIVE failed, errno %d\n", errno);
        }

        if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(enabled)) < 0)
        {
            printf("open_socket: setsockopt TCP_NODELAY failed, errno %d\n", errno);
        }

        if (ioctl(fd, FIONBIO, &enabled) < 0)
        {
            printf("ioctl: FIONBIO failed, errno %d\n", errno);
            close(fd);
            fd = -1;
        }
    }
    else
    {
        printf("Could not open tcp socket, errno %d\n", errno);
    }

    return fd;
}

static ccimp_status_t app_tcp_connect(int const fd, in_addr_t const ip_addr)
{

    struct sockaddr_in sin = {0};
    ccimp_status_t status = CCIMP_STATUS_OK;

    memcpy(&sin.sin_addr, &ip_addr, sizeof sin.sin_addr);
    sin.sin_port   = htons(CONNECTOR_PORT);
    sin.sin_family = AF_INET;

    printf("app_tcp_connect: fd %d\n", fd);


    if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        int const err = errno;
        switch (err)
        {
        case EINTR:
        case EAGAIN:
        case EINPROGRESS:
            status = CCIMP_STATUS_BUSY;
            break;

        default:
            printf("app_tcp_connect: connect() failed, fd %d, errno %d\n", fd, err);
            status = CCIMP_STATUS_ABORT;
        }
    }

    return status;
}

static ccimp_status_t app_is_tcp_connect_complete(int const fd)
{
    ccimp_status_t status = CCIMP_STATUS_BUSY;
    struct timeval timeout = {0};
    fd_set read_set, write_set;
    int rc;

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);
    write_set = read_set;

    rc = select(fd+1, &read_set, &write_set, NULL, &timeout);
    if (rc < 0)
    {
        if (errno != EINTR) {
            printf("app_is_tcp_connect_complete: select on fd %d returned %d, errno %d\n", fd, rc, errno);
            status = CCIMP_STATUS_ABORT;
        }
    }
    else
    /* Check whether the socket is now writable (connection succeeded). */
    if (rc > 0 && FD_ISSET(fd, &write_set))
    {
        /* We expect "socket writable" when the connection succeeds. */
        /* If we also got a "socket readable" we have an error. */
        if (FD_ISSET(fd, &read_set))
        {
            printf("app_is_tcp_connect_complete: FD_ISSET for read, fd %d\n", fd);
            status = CCIMP_STATUS_ABORT;

        }
        else
        {
            status = CCIMP_STATUS_OK;
        }
    }
    return status;
}

ccimp_status_t ccimp_network_tcp_open(ccimp_network_open_t * const data)
{
#define APP_CONNECT_TIMEOUT 30

    static unsigned long connect_time;
    static int fd = -1;
    socklen_t interface_addr_len;

    ccimp_status_t status = CCIMP_STATUS_ABORT;
    data->handle = &fd;

    if (fd == -1)
    {
        in_addr_t ip_addr;
        int const dns_resolve_error = dns_resolve(data->device_cloud.url, &ip_addr);
        if (dns_resolve_error != 0)
        {
            printf("app_network_tcp_open: Can't resolve DNS for %s\n", data->device_cloud.url);
            status = CCIMP_STATUS_ABORT;
            goto done;
        }

        fd = app_tcp_create_socket();
        if (fd == -1)
        {
            status = CCIMP_STATUS_ABORT;
            goto done;
        }

        {
            ccimp_os_system_up_time_t uptime;
            ccimp_os_get_system_time(&uptime);
            connect_time = uptime.sys_uptime;
        }
        status = app_tcp_connect(fd, ip_addr);
        if (status != CCIMP_STATUS_OK)
            goto error;
    }

    /* Get socket info of connected interface */
    interface_addr_len = sizeof(interface_addr);
    if (getsockname(fd, (struct sockaddr *)&interface_addr, &interface_addr_len))
    {
        printf("network_connect: getsockname error, errno %d\n", errno);
        goto done;
    }

    status = app_is_tcp_connect_complete(fd);
    if (status == CCIMP_STATUS_OK)
    {
         printf("app_network_tcp_open: connected to %s\n", data->device_cloud.url);
         goto done;
    }

    if (status == CCIMP_STATUS_BUSY)
    {
        unsigned long elapsed_time;
        ccimp_os_system_up_time_t uptime;

        ccimp_os_get_system_time(&uptime);
        elapsed_time = uptime.sys_uptime - connect_time;

        if (elapsed_time >= APP_CONNECT_TIMEOUT)
        {
            printf("app_network_tcp_open: failed to connect withing 30 seconds\n");
            status = CCIMP_STATUS_ABORT;
        }
    }

error:
    if (status == CCIMP_STATUS_ABORT)
    {
        printf("app_network_tcp_open: failed to connect to %s\n", data->device_cloud.url);
        dns_set_redirected(0);

        if (fd >= 0)
        {
            close(fd);
            fd = -1;
        }
    }

done:
    return status;
}