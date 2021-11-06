#ifndef IP_ARCH_H
#define IP_ARCH_H

/**
 * Architecture specific IP functions
 * The functions in the architecture specific module implement the IP
 * check sum and 32-bit additions.
 *
 * The IP checksum calculation is the most computationally expensive
 * operation in the TCP/IP stack and it therefore pays off to
 * implement this in efficient assembler. The purpose of the uip-arch
 * module is to let the checksum functions to be implemented in
 * architecture specific assembler.
 */

#include "ip.h"

/**
 * @brief Carry out a 32-bit addition.
 *
 * Because not all architectures for which IP is intended has native
 * 32-bit arithmetic, IP uses an external C function for doing the
 * required 32-bit additions in the TCP protocol processing. This
 * function should add the two arguments and place the result in the
 * global variable ip_acc32.
 *
 * \note The 32-bit integer pointed to by the op32 parameter and the
 * result in the ip_acc32 variable are in network byte order (big
 * endian).
 *
 * @param op32 A pointer to a 4-byte array representing a 32-bit
 * integer in network byte order (big endian).
 *
 * @param op16 A 16-bit integer in host byte order.
 */
void ip_add32(uint8_t *op32, uint16_t op16);

/**
 * @brief Calculate the Internet checksum over a buffer.
 *
 * The Internet checksum is the one's complement of the one's
 * complement sum of all 16-bit words in the buffer.
 *
 * See RFC1071.
 *
 * \note This function is not called in the current version of IP,
 * but future versions might make use of it.
 *
 * @param buf A pointer to the buffer over which the checksum is to be
 * computed.
 *
 * @param len The length of the buffer over which the checksum is to
 * be computed.
 *
 * \return The Internet checksum of the buffer.
 */
uint16_t ip_chksum(uint16_t *buf, uint16_t len);

/**
 * @brief Calculate the IP header checksum of the packet header in ip_buf.
 *
 * The IP header checksum is the Internet checksum of the 20 bytes of
 * the IP header.
 *
 * \return The IP header checksum of the IP header in the ip_buf
 * buffer.
 */
uint16_t ip_ipchksum(void);

/**
 * @brief Calculate the TCP checksum of the packet in ip_buf and ip_appdata.
 *
 * The TCP checksum is the Internet checksum of data contents of the
 * TCP segment, and a pseudo-header as defined in RFC793.
 *
 * \note The ip_appdata pointer that points to the packet data may
 * point anywhere in memory, so it is not possible to simply calculate
 * the Internet checksum of the contents of the ip_buf buffer.
 *
 * \return The TCP checksum of the TCP segment in ip_buf and pointed
 * to by ip_appdata.
 */
uint16_t ip_tcpchksum(void);

uint16_t ip_udpchksum(void);

#endif /* IP_ARCH_H */ 