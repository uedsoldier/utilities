/**
 * @file ip.h
 * @brief Header file for the uIP TCP/IP stack.
 * @author Adam Dunkels <adam@dunkels.com>
 *
 * The uIP TCP/IP stack header file contains definitions for a number
 * of C macros that are used by uIP programs as well as internal uIP
 * structures, TCP/IP header structures and function declarations.
 *
 */

#ifndef IP_H
#define IP_H

/**
 * Representación de una dirección IP
*/
#if IP_CONF_IPV6
// TODO
typedef IPV6_address_t IP_address_t 
#else /* IP_CONF_IPV6 */
typedef IPV4_address_t IP_address_t 
#endif


#endif /*IP_H*/