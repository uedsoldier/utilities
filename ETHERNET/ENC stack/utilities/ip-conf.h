#ifndef IP_CONF_H
#define IP_CONF_H

#include <stdint.h>
#include "ipethernet-conf.h"

/**
 * @brief Statistics datatype
 * This typedef defines the dataype used for keeping statistics in uIP
*/
typedef uint16_t ip_stats_t


/**
 * @brief Maximum number of listening TCP ports.
 */
#define IP_CONF_MAX_LISTENPORTS 4

/**
 * @brief IP buffer size.
 */
#define IP_CONF_BUFFER_SIZE     98

/**
 * @brief The TCP maximum segment size.
 * This is should not be to set to more than
 * IP_BUFSIZE - IP_LLH_LEN - IP_TCPIP_HLEN.
 */

#define IP_CONF_TCP_MSS 512

/**
 * @brief The size of the advertised receiver's window.
 * Should be set low (i.e., to the size of the uip_buf buffer) is the
 * application is slow to process incoming data, or high (32768 bytes)
 * if the application processes data quickly.
 */
#define IP_CONF_RECEIVE_WINDOW 512

/**
 * @brief CPU byte order.
 */
#define IP_CONF_BYTE_ORDER      __BYTE_ORDER__

/**
 * @brief Logging on or off
 */
#define IP_CONF_LOGGING         0


/**
 * @brief UDP support on or off
 */
#define IP_CONF_UDP             1

/**
 * @brief Maximum number of listening UDP ports.
 */
#define IP_CONF_UDP_CONNS       4

/**
 * @brief UDP checksums on or off
 */
#define IP_CONF_UDP_CHECKSUMS   1


/**
 * @brief UDP Broadcast (receive) on or off
 */
#define UIP_CONF_BROADCAST    1


/**
 * @brief IP statistics on or off
 */
#define UIP_CONF_STATISTICS      0

// SLIP
//#define IP_CONF_LLH_LEN 0

typedef void* ip_tcp_appstate_t;

void ipclient_appcall(void);

#define UIP_APPCALL ipclient_appcall

typedef void* ip_udp_appstate_t;

void ipudp_appcall(void);

#define IP_UDP_APPCALL ipudp_appcall

#define CC_REGISTER_ARG register

#define UIP_ARCH_CHKSUM 1


#endif /*IP_CONF_H*/