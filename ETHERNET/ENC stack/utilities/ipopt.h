#ifndef IPOPT_H
#define IPOPT_H

/**
 * Endianess
 */
#ifndef IP_LITTLE_ENDIAN
    #if defined(LITTLE_ENDIAN)
    #define IP_LITTLE_ENDIAN LITTLE_ENDIAN
    #elif defined(__ORDER_LITTLE_ENDIAN__)
    #define IP_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
    #else
    #define IP_LITTLE_ENDIAN 1234
    #endif
    #endif /* IP_LITTLE_ENDIAN */
    #ifndef IP_BIG_ENDIAN
    #if defined(BIG_ENDIAN)
    #define IP_BIG_ENDIAN BIG_ENDIAN
    #elif defined(__ORDER_BIG_ENDIAN__)
    #define IP_BIG_ENDIAN __ORDER_BIG_ENDIAN__
    #else
    #define IP_BIG_ENDIAN 4321
    #endif
#endif /* IP_BIG_ENDIAN */

#include "ip-conf.h"


/**
 * Static configuration options
 * These configuration options can be used for setting the IP address
 * settings statically, but only if IP_FIXEDADDR is set to 1. The
 * configuration options for a specific node includes IP address,
 * netmask and default router as well as the Ethernet address. The
 * netmask, default router and Ethernet address are appliciable only
 * if IP should be run over Ethernet.
 *
 * All of these should be changed to suit your project.
*/

/**
 * @brief Determines if IP should use a fixed IP address or not.
 * If IP should use a fixed IP address, the settings are set in the
 * uipopt.h file. If not, the macros IP_sethostaddr(),
 * IP_setdraddr() and IP_setnetmask() should be used instead.

*/
#define IP_FIXED_ADDRESS 0

/**
 * @brief Ping IP address asignment.
 * IP uses a "ping" packets for setting its own IP address if this
 * option is set. If so, IP will start with an empty IP address and
 * the destination IP address of the first incoming "ping" (ICMP echo)
 * packet will be used for setting the hosts IP address.
 * 
 * @note This works only if IP_FIXEDADDR is 0.
 */
#ifdef IP_CONF_PINGADDRCONF
#define IP_PINGADDRCONF IP_CONF_PINGADDRCONF
#else /* IP_CONF_PINGADDRCONF */
#define IP_PINGADDRCONF 0
#endif /* IP_CONF_PINGADDRCONF */

/**
 * @brief Specifies if the IP ARP module should be compiled with a fixed
 * Ethernet MAC address or not.
 *
 * If this configuration option is 0, the macro IP_setethaddr() can
 * be used to specify the Ethernet address at run-time.
 *

 */
#define IP_FIXED_ETHADDR 0

/**
 * Opciones de configuración IP
*/

/**
 * @brief The IP TTL (time to live) of IP packets sent by IP.
 * This should normally not be changed.

*/
#define IP_TTL  64

/**
 * @brief Turn on support for IP packet reassembly.
 *
 * IP supports reassembly of fragmented IP packets. This features
 * requires an additonal amount of RAM to hold the reassembly buffer
 * and the reassembly code size is approximately 700 bytes.  The
 * reassembly buffer is of the same size as the IP_buf buffer
 * (configured by IP_BUFSIZE).
 *
 * @note IP packet reassembly is not heavily tested.
 *

 */
#define IP_REASSEMBLY   0

/**
 * @brief The maximum time an IP fragment should wait in the reassembly
 * buffer before it is dropped.
 */
#define IP_REASS_MAXAGE 40

/**
 * Opciones de configuración UDP
*/

/**
 * @brief Toggles wether UDP support should be compiled in or not.

 */
#ifdef IP_CONF_UDP
#define IP_UDP IP_CONF_UDP
#else /* IP_CONF_UDP */
#define IP_UDP           0
#endif /* IP_CONF_UDP */


/**
 * @brief Toggles if UDP checksums should be used or not.
 * @note Support for UDP checksums is currently not included in IP,
 * so this option has no function.

 */
#ifdef IP_CONF_UDP_CHECKSUMS
#define IP_UDP_CHECKSUMS IP_CONF_UDP_CHECKSUMS
#else
#define IP_UDP_CHECKSUMS 0
#endif

/**
 * @brief The maximum amount of concurrent UDP connections.
 *
 */
#ifdef IP_CONF_UDP_CONNS
#define IP_UDP_CONNS IP_CONF_UDP_CONNS
#else /* IP_CONF_UDP_CONNS */
#define IP_UDP_CONNS    10
#endif /* IP_CONF_UDP_CONNS */

/**
 * Opciones de configuracion TCP
 */

/**
 * @brief Determines if support for opening connections from IP should be
 * compiled in.
 *
 * If the applications that are running on top of IP for this project
 * do not need to open outgoing TCP connections, this configration
 * option can be turned off to reduce the code size of IP.
 *
 */
#define IP_ACTIVE_OPEN 1

/**
 * @brief The maximum number of simultaneously open TCP connections.
 *
 * Since the TCP connections are statically allocated, turning this
 * configuration knob down results in less RAM used. Each TCP
 * connection requires approximatly 30 bytes of memory.
 *

 */
#ifndef IP_CONF_MAX_CONNECTIONS
#define IP_CONNS       10
#else /* IP_CONF_MAX_CONNECTIONS */
#define IP_CONNS IP_CONF_MAX_CONNECTIONS
#endif /* IP_CONF_MAX_CONNECTIONS */


/**
 * @brief The maximum number of simultaneously listening TCP ports.
 *
 * Each listening TCP port requires 2 bytes of memory.
 *

 */
#ifndef IP_CONF_MAX_LISTENPORTS
#define IP_LISTENPORTS 20
#else /* IP_CONF_MAX_LISTENPORTS */
#define IP_LISTENPORTS IP_CONF_MAX_LISTENPORTS
#endif /* IP_CONF_MAX_LISTENPORTS */

/**
 * @brief Determines if support for TCP urgent data notification should be
 * compiled in.
 *
 * Urgent data (out-of-band data) is a rarely used TCP feature that
 * very seldom would be required.
 *

 */
#define IP_URGDATA      0

/**
 * @brief The initial retransmission timeout counted in timer pulses.
 *
 * This should not be changed.
 */
#define IP_RTO         3

/**
 * @brief The maximum number of times a segment should be retransmitted
 * before the connection should be aborted.
 *
 * This should not be changed.
 */
#define IP_MAXRTX      8

/**
 * @brief The maximum number of times a SYN segment should be retransmitted
 * before a connection request should be deemed to have been
 * unsuccessful.
 *
 * This should not need to be changed.
 */
#define IP_MAXSYNRTX      5

/**
 * @brief The TCP maximum segment size.
 *
 * This is should not be to set to more than
 * IP_BUFSIZE - IP_LLH_LEN - IP_TCPIP_HLEN.
 */
#ifndef IP_CONF_TCP_MSS
#define IP_TCP_MSS     (IP_BUFSIZE - IP_LLH_LEN - IP_TCPIP_HLEN)
#else
#define IP_TCP_MSS IP_CONF_TCP_MSS
#endif

/**
 * @brief The size of the advertised receiver's window.
 *
 * Should be set low (i.e., to the size of the uip_buf buffer) is the
 * application is slow to process incoming data, or high (32768 bytes)
 * if the application processes data quickly.
 *
 */
#ifndef IP_CONF_RECEIVE_WINDOW
#define IP_RECEIVE_WINDOW IP_TCP_MSS
#else
#define IP_RECEIVE_WINDOW IP_CONF_RECEIVE_WINDOW
#endif

/**
 * @brief How long a connection should stay in the TIME_WAIT state.
 *
 * This configiration option has no real implication, and it should be
 * left untouched.
 */
#define IP_TIME_WAIT_TIMEOUT 120

/**
 * Opciones de configuracion ARP
 * 
 */

/**
 * The size of the ARP table.
 *
 * This option should be set to a larger value if this IP node will
 * have many connections from the local network.
 *
 */
#ifdef IP_CONF_ARPTAB_SIZE
#define IP_ARPTAB_SIZE IP_CONF_ARPTAB_SIZE
#else
#define IP_ARPTAB_SIZE 8
#endif

/**
 * The maxium age of ARP table entries measured in 10ths of seconds.
 *
 * An IP_ARP_MAXAGE of 120 corresponds to 20 minutes (BSD
 * default).
 */
#define IP_ARP_MAXAGE 120

/**
 * Opciones de configuracion general
 * 
 */

/**
 * @brief The size of the IP packet buffer.
 *
 * The IP packet buffer should not be smaller than 60 bytes, and does
 * not need to be larger than 1500 bytes. Lower size results in lower
 * TCP throughput, larger size results in higher TCP throughput.
 *
 */
#ifndef IP_CONF_BUFFER_SIZE
#define IP_BUFSIZE     400
#else /* IP_CONF_BUFFER_SIZE */
#define IP_BUFSIZE IP_CONF_BUFFER_SIZE
#endif /* IP_CONF_BUFFER_SIZE */


/**
 * @brief Determines if statistics support should be compiled in.
 *
 * The statistics is useful for debugging and to show the user.
 */
#ifndef IP_CONF_STATISTICS
#define IP_STATISTICS  0
#else /* IP_CONF_STATISTICS */
#define IP_STATISTICS IP_CONF_STATISTICS
#endif /* IP_CONF_STATISTICS */

/**
 * @brief Determines if logging of certain events should be compiled in.
 *
 * This is useful mostly for debugging. The function uip_log()
 * must be implemented to suit the architecture of the project, if
 * logging is turned on.
 *
 */
#ifndef IP_CONF_LOGGING
#define IP_LOGGING     0
#else /* IP_CONF_LOGGING */
#define IP_LOGGING     IP_CONF_LOGGING
#endif /* IP_CONF_LOGGING */

/**
 * @brief Broadcast support.
 *
 * This flag configures IP broadcast support. This is useful only
 * together with UDP.
 *
 *
 */
#if IP_UDP && IP_CONF_BROADCAST
#define IP_BROADCAST IP_CONF_BROADCAST
#else /* IP_CONF_BROADCAST */
#define IP_BROADCAST 0
#endif /* IP_CONF_BROADCAST */

/**
 * @brief Print out a IP log message.
 *
 * This function must be implemented by the module that uses IP, and
 * is called by IP whenever a log message is generated.
 */
void uip_log(char *msg);

/**
 * @brief The link level header length.
 *
 * This is the offset into the uip_buf where the IP header can be
 * found. For Ethernet, this should be set to 14. For SLIP, this
 * should be set to 0.
 *
 */
#ifdef IP_CONF_LLH_LEN
#define IP_LLH_LEN IP_CONF_LLH_LEN
#else /* IP_CONF_LLH_LEN */
#define IP_LLH_LEN     14
#endif /* IP_CONF_LLH_LEN */

/**
 * Configuracion de arquitectura de CPU
 * The CPU architecture configuration is where the endianess of the
 * CPU on which uIP is to be run is specified. Most CPUs today are
 * little endian, and the most notable exception are the Motorolas
 * which are big endian. The BYTE_ORDER macro should be changed to
 * reflect the CPU architecture on which uIP is to be run.
 */

/**
 * @brief The byte order of the CPU architecture on which uIP is to be run.
 *
 * This option can be either BIG_ENDIAN (Motorola byte order) or
 * LITTLE_ENDIAN (Intel byte order).
 *
 */
#ifdef IP_CONF_BYTE_ORDER
#define IP_BYTE_ORDER     IP_CONF_BYTE_ORDER
#else /* IP_CONF_BYTE_ORDER */
#define IP_BYTE_ORDER     IP_LITTLE_ENDIAN
#endif /* IP_CONF_BYTE_ORDER */

/**
 * Opciones de configuracion especificas por aplicacion
 * 
 */

/**
 * \name Appication specific configurations
 * @{
 *
 * An uIP application is implemented using a single application
 * function that is called by uIP whenever a TCP/IP event occurs. The
 * name of this function must be registered with uIP at compile time
 * using the UIP_APPCALL definition.
 *
 * uIP applications can store the application state within the
 * uip_conn structure by specifying the type of the application
 * structure by typedef:ing the type uip_tcp_appstate_t and uip_udp_appstate_t.
 *
 * The file containing the definitions must be included in the
 * uipopt.h file.
 *
 * The following example illustrates how this can look.
 \code
void httpd_appcall(void);
#define UIP_APPCALL     httpd_appcall
struct httpd_state {
  u8_t state;
  u16_t count;
  char *dataptr;
  char *script;
};
typedef struct httpd_state uip_tcp_appstate_t
 \endcode
 */

/**
 * \var #define UIP_APPCALL
 *
 * The name of the application function that uIP should call in
 * response to TCP/IP events.
 *
 */

/**
 * \var typedef uip_tcp_appstate_t
 *
 * The type of the application state that is to be stored in the
 * uip_conn structure. This usually is typedef:ed to a struct holding
 * application state information.
 */

/**
 * \var typedef uip_udp_appstate_t
 *
 * The type of the application state that is to be stored in the
 * uip_conn structure. This usually is typedef:ed to a struct holding
 * application state information.
 */
/** @} */
/** @} */



#endif /*IPOPT_H*/