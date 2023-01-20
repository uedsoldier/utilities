/**
 * @file socket.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef SOCKET_H
#define SOCKET_H

#ifdef __cplusplus
extern "C"
{
#endif

#pragma region Dependencies
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../../INTERNET/IPV4/IPv4.h"
#pragma endregion

#pragma region Useful macros
/**
 * @brief
 *
 */
#define SOCKET uint8_t

/**
 * @brief
 *
 */
#define CHECK_SOCKNUM(socket_number)            \
    do                                          \
    {                                           \
        if (socket_number > _WIZCHIP_SOCK_NUM_) \
            return SOCKERR_SOCKNUM;             \
    } while (0);

/**
 * @brief
 *
 */
#define CHECK_SOCKMODE(socket_number, mode)           \
    do                                                \
    {                                                 \
        if ((getSn_MR(socket_number) & 0x0F) != mode) \
            return SOCKERR_SOCKMODE;                  \
    } while (0);

/**
 * @brief
 *
 */
#define CHECK_SOCKINIT(socket_number)               \
    do                                              \
    {                                               \
        if ((getSn_SR(socket_number) != SOCK_INIT)) \
            return SOCKERR_SOCKINIT;                \
    } while (0);

/**
 * @brief
 *
 */
#define CHECK_SOCKDATA(len)         \
    do                              \
    {                               \
        if (len == 0)               \
            return SOCKERR_DATALEN; \
    } while (0);

/**
 * @brief Socket Block IO Mode in @ref setsockopt().
 *
 */
#define SOCK_IO_BLOCK 0

/**
 * @brief Socket Non-block IO Mode in @ref setsockopt().
 *
 */
#define SOCK_IO_NONBLOCK 1

#pragma endregion

#pragma region Custom types
    /**
     * @brief
     *
     */
    typedef enum socket_error
    {
        SOCK_BUSY = 1, // Result is OK about socket process.
        SOCK_OK = 0,   // Socket is busy on processing the operation. Valid only Non-block IO Mode.

        SOCK_ERROR = -1,
        SOCKERR_SOCKNUM = (SOCK_ERROR - 1),    // Invalid socket number
        SOCKERR_SOCKOPT = (SOCK_ERROR - 2),    // Invalid socket option
        SOCKERR_SOCKINIT = (SOCK_ERROR - 3),   // Socket is not initialized or SIPR is Zero IP address when Sn_MR_TCP
        SOCKERR_SOCKCLOSED = (SOCK_ERROR - 4), // Socket unexpectedly closed.
        SOCKERR_SOCKMODE = (SOCK_ERROR - 5),   // Invalid socket mode for socket operation.
        SOCKERR_SOCKFLAG = (SOCK_ERROR - 6),   // Invalid socket flag
        SOCKERR_SOCKSTATUS = (SOCK_ERROR - 7), // Invalid socket status for socket operation.
        SOCKERR_ARG = (SOCK_ERROR - 10),       // Invalid argument.
        SOCKERR_PORTZERO = (SOCK_ERROR - 11),  // Port number is zero
        SOCKERR_IPINVALID = (SOCK_ERROR - 12), // Invalid IP address
        SOCKERR_TIMEOUT = (SOCK_ERROR - 13),   // Timeout occurred
        SOCKERR_DATALEN = (SOCK_ERROR - 14),   // Data length is zero or greater than buffer max size.
        SOCKERR_BUFFER = (SOCK_ERROR - 15),    // Socket buffer is not enough for data communication.
        SOCK_FATAL = (SOCK_ERROR - 16),        // Result is fatal error about socket process.
        SOCKFATAL_PACKLEN = (SOCK_ERROR - 17)  // Invalid packet length. Fatal Error.
    } socket_error_t;

    /**
     * @brief 
     * TODO 
     */
    typedef enum socket_protocol
    {
        TCP,
        UDP,
        MACRAW
    } socket_protocol_t;

    /**
     * @brief
     *
     */
    typedef enum socket_flag
    {
        SF_ETHER_OWN = (Sn_MR_MFEN),    ///< In @ref Sn_MR_MACRAW, Receive only the packet as broadcast, multicast and own packet
        SF_IGMP_VER2 = (Sn_MR_MC),      ///< In @ref Sn_MR_UDP with \ref SF_MULTI_ENABLE, Select IGMP version 2.
        SF_TCP_NODELAY = (Sn_MR_ND),    ///< In @ref Sn_MR_TCP, Use to nodelayed ack.
        SF_MULTI_ENABLE = (Sn_MR_MULTI), ///< In @ref Sn_MR_UDP, Enable multicast mode.

#if _WIZCHIP_ == 5500
        SF_BROAD_BLOCK = (Sn_MR_BCASTB), ///< In @ref Sn_MR_UDP or @ref Sn_MR_MACRAW, Block broadcast packet. Valid only in W5500
        SF_MULTI_BLOCK = (Sn_MR_MMB),    ///< In @ref Sn_MR_MACRAW, Block multicast packet. Valid only in W5500
        SF_IPv6_BLOCK = (Sn_MR_MIP6B),   ///< In @ref Sn_MR_MACRAW, Block IPv6 packet. Valid only in W5500
        SF_UNI_BLOCK = (Sn_MR_UCASTB),   ///< In @ref Sn_MR_UDP with \ref SF_MULTI_ENABLE. Valid only in W5500
#endif

// A201505 : For W5300
#if _WIZCHIP_ == 5300
        SF_TCP_ALIGN = 0x02, ///< Valid only \ref Sn_MR_TCP and W5300, refer to \ref Sn_MR_ALIGN
#endif

        SF_IO_NONBLOCK = 0x01 ///< Socket nonblock io mode. It used parameter in \ref socket().
    } socket_flag_t;

    /**
     * @brief The kind of Socket Interrupt.
     *
     */
    typedef enum
    {
        SIK_CONNECTED = (1 << 0),    ///< connected
        SIK_DISCONNECTED = (1 << 1), ///< disconnected
        SIK_RECEIVED = (1 << 2),     ///< data received
        SIK_TIMEOUT = (1 << 3),      ///< timeout occurred
        SIK_SENT = (1 << 4),         ///< send ok
        SIK_ALL = 0x1F               ///< all interrupt
    } sockint_kind;

    /**
     * @brief The type of @ref ctlsocket().
     *
     */
    typedef enum
    {
        CS_SET_IOMODE,    ///< set socket IO mode with @ref SOCK_IO_BLOCK or @ref SOCK_IO_NONBLOCK
        CS_GET_IOMODE,    ///< get socket IO mode
        CS_GET_MAXTXBUF,  ///< get the size of socket buffer allocated in TX memory
        CS_GET_MAXRXBUF,  ///< get the size of socket buffer allocated in RX memory
        CS_CLR_INTERRUPT, ///< clear the interrupt of socket with @ref sockint_kind
        CS_GET_INTERRUPT, ///< get the socket interrupt. refer to @ref sockint_kind
#if _WIZCHIP_ > 5100
        CS_SET_INTMASK, ///< set the interrupt mask of socket with @ref sockint_kind, Not supported in W5100
        CS_GET_INTMASK  ///< get the masked interrupt of socket. refer to @ref sockint_kind, Not supported in W5100
#endif
    } ctlsock_type;

    /**
     * @brief The type of socket option in @ref setsockopt() or @ref getsockopt()
     *
     */
    typedef enum
    {
        SO_FLAG,     ///< Valid only in getsockopt(), For set flag of socket refer to <I>flag</I> in @ref socket().
        SO_TTL,      ///< Set TTL. @ref Sn_TTL  ( @ref setSn_TTL(), @ref getSn_TTL() )
        SO_TOS,      ///< Set TOS. @ref Sn_TOS  ( @ref setSn_TOS(), @ref getSn_TOS() )
        SO_MSS,      ///< Set MSS. @ref Sn_MSSR ( @ref setSn_MSSR(), @ref getSn_MSSR() )
        SO_DESTIP,   ///< Set the destination IP address. @ref Sn_DIPR ( @ref setSn_DIPR(), @ref getSn_DIPR() )
        SO_DESTPORT, ///< Set the destination Port number. @ref Sn_DPORT ( @ref setSn_DPORT(), @ref getSn_DPORT() )
#if _WIZCHIP_ != 5100
        SO_KEEPALIVESEND, ///< Valid only in setsockopt. Manually send keep-alive packet in TCP mode, Not supported in W5100
#if !((_WIZCHIP_ == 5100) || (_WIZCHIP_ == 5200))
        SO_KEEPALIVEAUTO, ///< Set/Get keep-alive auto transmission timer in TCP mode, Not supported in W5100, W5200
#endif
#endif
        SO_SENDBUF,    ///< Valid only in getsockopt. Get the free data size of Socekt TX buffer. @ref Sn_TX_FSR, @ref getSn_TX_FSR()
        SO_RECVBUF,    ///< Valid only in getsockopt. Get the received data size in socket RX buffer. @ref Sn_RX_RSR, @ref getSn_RX_RSR()
        SO_STATUS,     ///< Valid only in getsockopt. Get the socket status. @ref Sn_SR, @ref getSn_SR()
        SO_REMAINSIZE, ///< Valid only in getsockopt. Get the remained packet size in other then TCP mode.
        SO_PACKINFO    ///< Valid only in getsockopt. Get the packet information as @ref PACK_FIRST, @ref PACK_REMAINED, and @ref PACK_COMPLETED in other then TCP mode.
    } sockopt_type;

#pragma endregion

#pragma region Function prototypes
    socket_error_t socket(uint8_t socket_number, socket_protocol_t protocol, uint16_t port, socket_flag_t flag);
    socket_error_t close(uint8_t socket_number);
    socket_error_t listen(uint8_t socket_number);
    socket_error_t connect(uint8_t socket_number, IPV4_address_t address, uint16_t port);
    socket_error_t disconnect(uint8_t socket_number);

    socket_error_t send(uint8_t socket_number, uint8_t *buffer, uint16_t len, uint32_t *sent_size);
    socket_error_t recv(uint8_t socket_number, uint8_t *buffer, uint16_t len, uint32_t *recv_size);

    socket_error_t sendto(uint8_t socket_number, uint8_t *buffer, uint16_t len, IPV4_address_t address, uint16_t port, uint32_t *sent_size);
    socket_error_t recvfrom(uint8_t socket_number, uint8_t *buffer, uint16_t len, IPV4_address_t address, uint16_t port, uint32_t *recv_size);

    socket_error_t ctlsocket(uint8_t socket_number, ctlsock_type cstype, void *arg);
    socket_error_t setockopt(uint8_t socket_number, sockopt_type sotype, void *arg);
    socket_error_t getsockopt(uint8_t socket_number, sockopt_type sotype, void *arg);
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif