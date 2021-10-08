#ifndef IP_CONF_H
#define IP_CONF_H

#include <stdint.h>
#include "ipethernet-conf.h"

/**
 * Statistics datatype
 * This typedef defines the dataype used for keeping statistics in uIP
*/
typedef uint16_t ip_stats_t


/**
 * Maximum number of listening TCP ports.
 * @hideinitializer
 */
#define IP_CONF_MAX_LISTENPORTS 4

/**
 * IP buffer size.
 * @hideinitializer
 */
#define IP_CONF_BUFFER_SIZE     98

/**
 * The TCP maximum segment size.
 * This is should not be to set to more than
 * IP_BUFSIZE - IP_LLH_LEN - IP_TCPIP_HLEN.
 */

#define IP_CONF_TCP_MSS 512

/**
 * The size of the advertised receiver's window.
 * Should be set low (i.e., to the size of the uip_buf buffer) is the
 * application is slow to process incoming data, or high (32768 bytes)
 * if the application processes data quickly.
 *
 * @hideinitializer
 */
#define IP_CONF_RECEIVE_WINDOW 512

/**
 * CPU byte order.
 *
 * @hideinitializer
 */
#define IP_CONF_BYTE_ORDER      __BYTE_ORDER__

/**
 * Logging on or off
 * @hideinitializer
 */
#define IP_CONF_LOGGING         0

#endif /*IP_CONF_H*/