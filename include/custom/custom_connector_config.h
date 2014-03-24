/*
 * ccapi_connector_config.h
 *
 *  Created on: Mar 6, 2014
 *      Author: hbujanda
 */

#ifndef _CUSTOM_CONNECTOR_CONFIG_H_
#define _CUSTOM_CONNECTOR_CONFIG_H_

/* Cloud Connector Configuration Categories */

/* Transports */
#undef  CCIMP_UDP_TRANSPORT_ENABLED
#undef  CCIMP_SMS_TRANSPORT_ENABLED

/* Services */
#define CCIMP_DATA_SERVICE_ENABLED
#define CCIMP_DATA_POINTS_ENABLED
#define CCIMP_RCI_SERVICE_ENABLED
#define CCIMP_FIRMWARE_SERVICE_ENABLED
#define CCIMP_FILE_SYSTEM_SERVICE_ENABLED

/* OS Features */
#define CCIMP_LITTLE_ENDIAN
#define CCIMP_COMPRESSION_ENABLED
#define CCIMP_64_BIT_INTEGERS_SUPPORTED
#define CCIMP_FLOATING_POINT_SUPPORTED

#define CCIMP_HAS_STDINT_HEADER

/* Logging */
#define CCIMP_LOGGING_ENABLED

/* Limits */
#define CCIMP_FILE_SYSTEM_MAX_PATH_LENGTH   256
#undef  CCIMP_FILE_SYSTEM_LARGE_FILES_SUPPORTED

#define CCIMP_SM_UDP_MAX_RX_SEGMENTS   256
#define CCIMP_SM_SMS_MAX_RX_SEGMENTS   256

#endif /* _CUSTOM_CONNECTOR_CONFIG_H_ */
