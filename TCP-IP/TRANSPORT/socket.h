/**
 * @file socket.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-12-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_
#ifdef __cplusplus
extern "C"
{
#endif


#pragma region Dependencies
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../DATA_LINK/ETHERNET/WIZNET/w5500.h"
#pragma endregion

/**
 * @brief 
 * 
 */
typedef uint8_t SOCKET_t;

/**
 * @brief 
 * 
 */
typedef enum {
    SOCKET_BUSY = 1, ///< Socket is busy on processing the operation. Valid only Non-block IO Mode.
    SOCKET_OK = 0,   ///< Result is OK about socket process.
    SOCKET_ERROR = -1,
    SOCKERR_SOCKNUM = (SOCKET_ERROR - 1),       ///< Invalid socket number
    SOCKERR_SOCKOPT = (SOCKET_ERROR - 2),       ///< Invalid socket option
    SOCKERR_SOCKINIT = (SOCKET_ERROR - 3),      ///< Socket is not initialized or SIPR is Zero IP address when Sn_MR_TCP
    SOCKERR_SOCKCLOSED = (SOCKET_ERROR - 4),    ///< Socket unexpectedly closed.
    SOCKERR_SOCKMODE = (SOCKET_ERROR - 5),      ///< Invalid socket mode for socket operation.
    SOCKERR_SOCKFLAG = (SOCKET_ERROR - 6),      ///< Invalid socket flag
    SOCKERR_SOCKSTATUS = (SOCKET_ERROR - 7),    ///< Invalid socket status for socket operation.
    SOCKERR_ARG = (SOCKET_ERROR - 10),          ///< Invalid argument.
    SOCKERR_PORTZERO = (SOCKET_ERROR - 11),     ///< Port number is zero
    SOCKERR_IPINVALID = (SOCKET_ERROR - 12),    ///< Invalid IP address
    SOCKERR_TIMEOUT = (SOCKET_ERROR - 13),      ///< Timeout occurred
    SOCKERR_DATALEN = (SOCKET_ERROR - 14),      ///< Data length is zero or greater than buffer max size.
    SOCKERR_BUFFER = (SOCKET_ERROR - 15),       ///< Socket buffer is not enough for data communication.
    SOCKET_FATAL = -1000,                       ///< Result is fatal error about socket process.
    SOCKFATAL_PACKLEN = (SOCKET_FATAL - 1),     ///< Invalid packet length. Fatal Error.

} socket_error_t;

typedef enum
{
    SF_ETHER_OWN = (Sn_MR_MFEN),     ///< In @ref Sn_MR_MACRAW, Receive only the packet as broadcast, multicast and own packet
    SF_IGMP_VER2 = (Sn_MR_MC),       ///< In @ref Sn_MR_UDP with \ref SF_MULTI_ENABLE, Select IGMP version 2.
    SF_TCP_NODELAY = (Sn_MR_ND),     ///< In @ref Sn_MR_TCP, Use to nodelayed ack.
    SF_MULTI_ENABLE = (Sn_MR_MULTI), ///< In @ref Sn_MR_UDP, Enable multicast mode.

#if _WIZCHIP_ == 5500
    SF_BROAD_BLOCK = (Sn_MR_BCASTB), ///< In @ref Sn_MR_UDP or @ref Sn_MR_MACRAW, Block broadcast packet. Valid only in W5500
    SF_MULTI_BLOCK = (Sn_MR_MMB),    ///< In @ref Sn_MR_MACRAW, Block multicast packet. Valid only in W5500
    SF_IPv6_BLOCK = (Sn_MR_MIP6B),   ///< In @ref Sn_MR_MACRAW, Block IPv6 packet. Valid only in W5500
    SF_UNI_BLOCK = (Sn_MR_UCASTB),   ///< In @ref Sn_MR_UDP with \ref SF_MULTI_ENABLE. Valid only in W5500

#if _WIZCHIP_ == 5300
    SF_TCP_ALIGN = 0x02, ///< Valid only \ref Sn_MR_TCP and W5300, refer to \ref Sn_MR_ALIGN
#endif

    SF_IO_NONBLOCK = 0x01 ///< Socket nonblock io mode. It used parameter in \ref socket().
#endif
} socket_flag_t;

#ifdef __cplusplus
}
#endif

#endif // _SOCKET_H_
