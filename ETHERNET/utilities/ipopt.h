#ifndef IPOPT_H
#define IPOPT_H

/**
 * Static configuration options
 * These configuration options can be used for setting the IP address
 * settings statically, but only if IP_FIXEDADDR is set to 1. The
 * configuration options for a specific node includes IP address,
 * netmask and default router as well as the Ethernet address. The
 * netmask, default router and Ethernet address are appliciable only
 * if uIP should be run over Ethernet.
 *
 * All of these should be changed to suit your project.
*/

/**
 * @brief Determines if uIP should use a fixed IP address or not.
 * If uIP should use a fixed IP address, the settings are set in the
 * uipopt.h file. If not, the macros IP_sethostaddr(),
 * IP_setdraddr() and IP_setnetmask() should be used instead.

*/
#define IP_FIXED_ADDRESS 0

/**
 * @brief Specifies if the uIP ARP module should be compiled with a fixed
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
 * @brief The IP TTL (time to live) of IP packets sent by uIP.
 * This should normally not be changed.

*/
#define IP_TTL  64

/**
 * @brief Turn on support for IP packet reassembly.
 *
 * uIP supports reassembly of fragmented IP packets. This features
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
 * @note Support for UDP checksums is currently not included in uIP,
 * so this option has no function.

 */
#ifdef IP_CONF_UDP_CHECKSUMS
#define IP_UDP_CHECKSUMS IP_CONF_UDP_CHECKSUMS
#else
#define IP_UDP_CHECKSUMS 0
#endif

#endif /*IPOPT_H*/