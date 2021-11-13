/**
 * @file ip.h
 * @brief Header file for the IP TCP/IP stack.
 * @author Adam Dunkels <adam@dunkels.com>
 *
 * The IP TCP/IP stack header file contains definitions for a number
 * of C macros that are used by IP programs as well as internal IP
 * structures, TCP/IP header structures and function declarations.
 *
 */

#ifndef IP_H
#define IP_H

#include <string.h>
#include "../../INTERNET/IPV4/IPv4.h"
#include "../../INTERNET/IPV6/IPv6.h"
#include "ipopt.h"

/**
 * Representación de una dirección IP
*/
#if IP_CONF_IPV6
// TODO
typedef IPV6_address_t IP_address
#else /* IP_CONF_IPV6 */
typedef IPV4_address_t IP_address
#define IP_ADDRESS_NONE 0x00000000
#endif

/**
 * @brief Set the IP address of this host.
 * The IP address is represented as a 4-byte array where the first
 * octet of the IP address is put in the first member of the 4-byte
 * array.
 *
 * Example:
   \code
   IP_address addr;
   ip_ipaddr(&addr, 192,168,1,2);
   ip_sethostaddr(&addr);

   \endcode
 * @param addr A pointer to an IP address of type IP_address;
 */
#define ip_sethostaddr(addr) ip_ipaddr_copy(ip_hostaddr, (addr))

/**
 * @brief Get the IP address of this host.
 * The IP address is represented as a 4-byte array where the first
 * octet of the IP address is put in the first member of the 4-byte
 * array.
 *
 * Example:
    \code
    IP_address hostaddr;
    ip_gethostaddr(&hostaddr);
    \endcode
 * @param addr A pointer to a IP_address variable that will be
 * filled in with the currently configured IP address.
 */
#define ip_gethostaddr(addr) ip_ipaddr_copy((addr), ip_hostaddr)

/**
 * @brief Set the default router's IP address.
 *
 * @param addr A pointer to a IP_address variable containing the IP
 * address of the default router.
 */
#define ip_setdraddr(addr) ip_ipaddr_copy(ip_draddr, (addr))

/**
 * @brief Set the netmask.
 * @param addr A pointer to a IP_address variable containing the IP
 * address of the netmask.
 */
#define ip_setnetmask(addr) ip_ipaddr_copy(ip_netmask, (addr))

/**
 * @brief Get the default router's IP address.
 * @param addr A pointer to a IP_address variable that will be
 * filled in with the IP address of the default router.
 */
#define ip_getdraddr(addr) ip_ipaddr_copy((addr), ip_draddr)

/**
 * @brief Get the netmask.
 * @param addr A pointer to a IP_address variable that will be
 * filled in with the value of the netmask.
 */
#define ip_getnetmask(addr) ip_ipaddr_copy((addr), ip_netmask)

   /**
 * @brief IP initialization function.
 * This function should be called at boot up to initilize the IP
 * TCP/IP stack.
 */
void ip_init(void);

/**
 * IP initialization function.
 *
 * This function may be used at boot time to set the initial ip_id.
 */
void ip_setipid(uint16_t id);

/**
 * @brief Process an incoming packet.
 * This function should be called when the device driver has received
 * a packet from the network. The packet from the device driver must
 * be present in the ip_buf buffer, and the length of the packet
 * should be placed in the ip_len variable.
 *
 * When the function returns, there may be an outbound packet placed
 * in the ip_buf packet buffer. If so, the ip_len variable is set to
 * the length of the packet. If no packet is to be sent out, the
 * ip_len variable is set to 0.
 *
 * The usual way of calling the function is presented by the source
 * code below.
    \code
    ip_len = devicedriver_poll();
    if(ip_len > 0) {
    ip_input();
    if(ip_len > 0) {
        devicedriver_send();
    }
    }
    \endcode
 *
 * \note If you are writing a IP device driver that needs ARP
 * (Address Resolution Protocol), e.g., when running IP over
 * Ethernet, you will need to call the IP ARP code before calling
 * this function:
    \code
    #define BUF ((struct ip_eth_hdr *)&ip_buf[0])
    ip_len = ethernet_devicedrver_poll();
    if(ip_len > 0) {
    if(BUF->type == HTONS(IP_ETHTYPE_IP)) {
        ip_arp_ipin();
        ip_input();
        if(ip_len > 0) {
        ip_arp_out();
    ethernet_devicedriver_send();
        }
    } else if(BUF->type == HTONS(IP_ETHTYPE_ARP)) {
        ip_arp_arpin();
        if(ip_len > 0) {
    ethernet_devicedriver_send();
        }
    }
    \endcode
 *
 */
#define ip_input() ip_process(IP_DATA)

/**
 * @brief Periodic processing for a connection identified by its number.
 *
 * This function does the necessary periodic processing (timers,
 * polling) for a IP TCP conneciton, and should be called when the
 * periodic IP timer goes off. It should be called for every
 * connection, regardless of whether they are open of closed.
 *
 * When the function returns, it may have an outbound packet waiting
 * for service in the IP packet buffer, and if so the ip_len
 * variable is set to a value larger than zero. The device driver
 * should be called to send out the packet.
 *
 * The ususal way of calling the function is through a for() loop like
 * this:
   \code
   for(i = 0; i < IP_CONNS; ++i) {
   ip_periodic(i);
   if(ip_len > 0) {
      devicedriver_send();
   }
   }
   \endcode
 *
 * \note If you are writing a IP device driver that needs ARP
 * (Address Resolution Protocol), e.g., when running IP over
 * Ethernet, you will need to call the ip_arp_out() function before
 * calling the device driver:
   \code
   for(i = 0; i < IP_CONNS; ++i) {
   ip_periodic(i);
   if(ip_len > 0) {
      ip_arp_out();
      ethernet_devicedriver_send();
   }
   }
   \endcode
 *
 * @param conn The number of the connection which is to be periodically polled.
 *
 */
#define ip_periodic(conn)        \
   do                            \
   {                             \
      ip_conn = &ip_conns[conn]; \
      ip_process(IP_TIMER);      \
   } while (0)

/**
 *
 *
 */
#define ip_conn_active(conn) (ip_conns[conn].tcpstateflags != IP_CLOSED)

/**
 * @brief Perform periodic processing for a connection identified by a pointer
 * to its structure.
 *
 * Same as ip_periodic() but takes a pointer to the actual ip_conn
 * struct instead of an integer as its argument. This function can be
 * used to force periodic processing of a specific connection.
 *
 * @param conn A pointer to the ip_conn struct for the connection to
 * be processed.
 */
#define ip_periodic_conn(conn) \
   do                          \
   {                           \
      ip_conn = conn;          \
      ip_process(IP_TIMER);    \
   } while (0)

/**
 * @brief Request that a particular connection should be polled.
 *
 * Similar to ip_periodic_conn() but does not perform any timer
 * processing. The application is polled for new data.
 *
 * @param conn A pointer to the ip_conn struct for the connection to
 * be processed.
 */
#define ip_poll_conn(conn)         \
   do                              \
   {                               \
      ip_conn = conn;              \
      ip_process(IP_POLL_REQUEST); \
   } while (0)

#if IP_UDP
/**
 * Periodic processing for a UDP connection identified by its number.
 *
 * This function is essentially the same as ip_periodic(), but for
 * UDP connections. It is called in a similar fashion as the
 * ip_periodic() function:
    \code
    for(i = 0; i < IP_UDP_CONNS; i++) {
    ip_udp_periodic(i);
    if(ip_len > 0) {
        devicedriver_send();
    }
    }
    \endcode
 *
 * \note As for the ip_periodic() function, special care has to be
 * taken when using IP together with ARP and Ethernet:
    \code
    for(i = 0; i < IP_UDP_CONNS; i++) {
    ip_udp_periodic(i);
    if(ip_len > 0) {
        ip_arp_out();
        ethernet_devicedriver_send();
    }
    }
    \endcode
 *
 * @param conn The number of the UDP connection to be processed.
 */
#define ip_udp_periodic(conn)            \
   do                                    \
   {                                     \
      ip_udp_conn = &ip_udp_conns[conn]; \
      ip_process(IP_UDP_TIMER);          \
   } while (0)

/**
 * Periodic processing for a UDP connection identified by a pointer to
 * its structure.
 *
 * Same as ip_udp_periodic() but takes a pointer to the actual
 * ip_conn struct instead of an integer as its argument. This
 * function can be used to force periodic processing of a specific
 * connection.
 *
 * @param conn A pointer to the ip_udp_conn struct for the connection
 * to be processed.
 */
#define ip_udp_periodic_conn(conn) \
   do                              \
   {                               \
      ip_udp_conn = conn;          \
      ip_process(IP_UDP_TIMER);    \
   } while (0)

#endif /* IP_UDP */

/**
 * @brief The IP packet buffer.
 *
 * The ip_buf array is used to hold incoming and outgoing
 * packets. The device driver should place incoming data into this
 * buffer. When sending data, the device driver should read the link
 * level headers and the TCP/IP headers from this buffer. The size of
 * the link level headers is configured by the IP_LLH_LEN define.
 *
 * \note The application data need not be placed in this buffer, so
 * the device driver must read it from the place pointed to by the
 * ip_appdata pointer as illustrated by the following example:
    \code
    void
    devicedriver_send(void)
    {
    hwsend(&ip_buf[0], IP_LLH_LEN);
    if(ip_len <= IP_LLH_LEN + IP_TCPIP_HLEN) {
        hwsend(&ip_buf[IP_LLH_LEN], ip_len - IP_LLH_LEN);
    } else {
        hwsend(&ip_buf[IP_LLH_LEN], IP_TCPIP_HLEN);
        hwsend(ip_appdata, ip_len - IP_TCPIP_HLEN - IP_LLH_LEN);
    }
    }
    \endcode
 */
extern uint8_t ip_buf[IP_BUFSIZE + 2];

/**
 * Start listening to the specified port.
 *
 * \note Since this function expects the port number in network byte
 * order, a conversion using HTONS() or htons() is necessary.
 *
    \code
    ip_listen(HTONS(80));
    \endcode
 *
 * @param port A 16-bit port number in network byte order.
 */
void ip_listen(uint16_t port);

/**
 * Stop listening to the specified port.
 *
 * \note Since this function expects the port number in network byte
 * order, a conversion using HTONS() or htons() is necessary.
 *
    \code
    ip_unlisten(HTONS(80));
    \endcode
 *
 * @param port A 16-bit port number in network byte order.
 */
void ip_unlisten(uint16_t port);

/**
 * Connect to a remote host using TCP.
 *
 * This function is used to start a new connection to the specified
 * port on the specied host. It allocates a new connection identifier,
 * sets the connection to the SYN_SENT state and sets the
 * retransmission timer to 0. This will cause a TCP SYN segment to be
 * sent out the next time this connection is periodically processed,
 * which usually is done within 0.5 seconds after the call to
 * ip_connect().
 *
 * \note This function is avaliable only if support for active open
 * has been configured by defining IP_ACTIVE_OPEN to 1 in uipopt.h.
 *
 * \note Since this function requires the port number to be in network
 * byte order, a conversion using HTONS() or htons() is necessary.
 *
   \code
   IP_address ipaddr;
   ip_ipaddr(&ipaddr, 192,168,1,2);
   ip_connect(&ipaddr, HTONS(80));
   \endcode
 *
 * @param ripaddr The IP address of the remote hot.
 *
 * @param port A 16-bit port number in network byte order.
 *
 * \return A pointer to the IP connection identifier for the new connection,
 * or NULL if no connection could be allocated.
 *
 */
struct ip_conn *ip_connect(IP_address *ripaddr, uint16_t port);

/**
 * \internal
 *
 * Check if a connection has outstanding (i.e., unacknowledged) data.
 *
 * @param conn A pointer to the ip_conn structure for the connection.
 *

 */
#define ip_outstanding(conn) ((conn)->len)

/**
 * Send data on the current connection.
 *
 * This function is used to send out a single segment of TCP
 * data. Only applications that have been invoked by IP for event
 * processing can send data.
 *
 * The amount of data that actually is sent out after a call to this
 * funcion is determined by the maximum amount of data TCP allows. IP
 * will automatically crop the data so that only the appropriate
 * amount of data is sent. The function ip_mss() can be used to query
 * IP for the amount of data that actually will be sent.
 *
 * \note This function does not guarantee that the sent data will
 * arrive at the destination. If the data is lost in the network, the
 * application will be invoked with the ip_rexmit() event being
 * set. The application will then have to resend the data using this
 * function.
 *
 * @param data A pointer to the data which is to be sent.
 *
 * @param len The maximum amount of data bytes to be sent.
 *

 */
void ip_send(const void *data, int len);

/**
 * The length of any incoming data that is currently avaliable (if avaliable)
 * in the ip_appdata buffer.
 *
 * The test function ip_data() must first be used to check if there
 * is any data available at all.
 *

 */
/*void ip_datalen(void);*/
#define ip_datalen() ip_len

/**
 * The length of any out-of-band data (urgent data) that has arrived
 * on the connection.
 *
 * \note The configuration parameter IP_URGDATA must be set for this
 * function to be enabled.
 *

 */
#define ip_urgdatalen() ip_urglen

/**
 * Close the current connection.
 *
 * This function will close the current connection in a nice way.
 *

 */
#define ip_close() (ip_flags = IP_CLOSE)

/**
 * Abort the current connection.
 *
 * This function will abort (reset) the current connection, and is
 * usually used when an error has occured that prevents using the
 * ip_close() function.
 *

 */
#define ip_abort() (ip_flags = IP_ABORT)

/**
 * Tell the sending host to stop sending data.
 *
 * This function will close our receiver's window so that we stop
 * receiving data for the current connection.
 *

 */
#define ip_stop() (ip_conn->tcpstateflags |= IP_STOPPED)

/**
 * Find out if the current connection has been previously stopped with
 * ip_stop().
 *

 */
#define ip_stopped(conn) ((conn)->tcpstateflags & IP_STOPPED)

/**
 * Restart the current connection, if is has previously been stopped
 * with ip_stop().
 *
 * This function will open the receiver's window again so that we
 * start receiving data for the current connection.
 *

 */
#define ip_restart()                         \
   do                                        \
   {                                         \
      ip_flags |= IP_NEWDATA;                \
      ip_conn->tcpstateflags &= ~IP_STOPPED; \
   } while (0)

/* IP tests that can be made to determine in what state the current
   connection is, and what the application function should do. */

/**
 * Is the current connection a UDP connection?
 *
 * This function checks whether the current connection is a UDP connection.
 *

 *
 */
#define ip_udpconnection() (ip_conn == NULL)

/**
 * Is new incoming data available?
 *
 * Will reduce to non-zero if there is new data for the application
 * present at the ip_appdata pointer. The size of the data is
 * avaliable through the ip_len variable.
 *

 */
#define ip_newdata() (ip_flags & IP_NEWDATA)

/**
 * Has previously sent data been acknowledged?
 *
 * Will reduce to non-zero if the previously sent data has been
 * acknowledged by the remote host. This means that the application
 * can send new data.
 *

 */
#define ip_acked() (ip_flags & IP_ACKDATA)

/**
 * Has the connection just been connected?
 *
 * Reduces to non-zero if the current connection has been connected to
 * a remote host. This will happen both if the connection has been
 * actively opened (with ip_connect()) or passively opened (with
 * ip_listen()).
 *

 */
#define ip_connected() (ip_flags & IP_CONNECTED)

/**
 * Has the connection been closed by the other end?
 *
 * Is non-zero if the connection has been closed by the remote
 * host. The application may then do the necessary clean-ups.
 *

 */
#define ip_closed() (ip_flags & IP_CLOSE)

/**
 * Has the connection been aborted by the other end?
 *
 * Non-zero if the current connection has been aborted (reset) by the
 * remote host.
 *

 */
#define ip_aborted() (ip_flags & IP_ABORT)

/**
 * Has the connection timed out?
 *
 * Non-zero if the current connection has been aborted due to too many
 * retransmissions.
 *

 */
#define ip_timedout() (ip_flags & IP_TIMEDOUT)

/**
 * Do we need to retransmit previously data?
 *
 * Reduces to non-zero if the previously sent data has been lost in
 * the network, and the application should retransmit it. The
 * application should send the exact same data as it did the last
 * time, using the ip_send() function.
 *

 */
#define ip_rexmit() (ip_flags & IP_REXMIT)

/**
 * Is the connection being polled by IP?
 *
 * Is non-zero if the reason the application is invoked is that the
 * current connection has been idle for a while and should be
 * polled.
 *
 * The polling event can be used for sending data without having to
 * wait for the remote host to send data.
 *

 */
#define ip_poll() (ip_flags & IP_POLL)

/**
 * Get the initial maxium segment size (MSS) of the current
 * connection.
 *

 */
#define ip_initialmss() (ip_conn->initialmss)

/**
 * Get the current maxium segment size that can be sent on the current
 * connection.
 *
 * The current maxiumum segment size that can be sent on the
 * connection is computed from the receiver's window and the MSS of
 * the connection (which also is available by calling
 * ip_initialmss()).
 *

 */
#define ip_mss() (ip_conn->mss)

/**
 * Set up a new UDP connection.
 *
 * This function sets up a new UDP connection. The function will
 * automatically allocate an unused local port for the new
 * connection. However, another port can be chosen by using the
 * ip_udp_bind() call, after the ip_udp_new() function has been
 * called.
 *
 * Example:
    \code
    IP_address addr;
    struct ip_udp_conn *c;

    ip_ipaddr(&addr, 192,168,2,1);
    c = ip_udp_new(&addr, HTONS(12345));
    if(c != NULL) {
    ip_udp_bind(c, HTONS(12344));
    }
    \endcode
 * @param ripaddr The IP address of the remote host.
 *
 * @param rport The remote port number in network byte order.
 *
 * \return The ip_udp_conn structure for the new connection or NULL
 * if no connection could be allocated.
 */
struct ip_udp_conn *ip_udp_new(IP_address *ripaddr, uint16_t rport);

/**
 * Removed a UDP connection.
 *
 * @param conn A pointer to the ip_udp_conn structure for the connection.
 *

 */
#define ip_udp_remove(conn) (conn)->lport = 0

/**
 * Bind a UDP connection to a local port.
 *
 * @param conn A pointer to the ip_udp_conn structure for the
 * connection.
 *
 * @param port The local port number, in network byte order.
 *

 */
#define ip_udp_bind(conn, port) (conn)->lport = port

/**
 * Send a UDP datagram of length len on the current connection.
 *
 * This function can only be called in response to a UDP event (poll
 * or newdata). The data must be present in the ip_buf buffer, at the
 * place pointed to by the ip_appdata pointer.
 *
 * @param len The length of the data in the ip_buf buffer.
 *

 */
#define ip_udp_send(len) ip_send((char *)ip_appdata, len)

//!

/**
 * Pointer to the application data in the packet buffer.
 *
 * This pointer points to the application data when the application is
 * called. If the application wishes to send data, the application may
 * use this space to write the data into before calling ip_send().
 */
extern void *ip_appdata;

#if IP_URGDATA > 0
/* uint8_t *ip_urgdata:
 *
 * This pointer points to any urgent data that has been received. Only
 * present if compiled with support for urgent data (IP_URGDATA).
 */
extern void *ip_urgdata;
#endif /* IP_URGDATA > 0 */

/**
 * Variables utilizadas en los controladores de dispositivos uIP
 */
/**
 * The length of the packet in the ip_buf buffer.
 *
 * The global variable ip_len holds the length of the packet in the
 * ip_buf buffer.
 *
 * When the network device driver calls the uIP input function,
 * ip_len should be set to the length of the packet in the ip_buf
 * buffer.
 *
 * When sending packets, the device driver should use the contents of
 * the ip_len variable to determine the length of the outgoing
 * packet.
 *
 */
extern uint16_t ip_len;

#if IP_URGDATA > 0
extern uint16_t ip_urglen, ip_surglen;
#endif /* IP_URGDATA > 0 */

/**
 * Representation of a uIP TCP connection.
 *
 * The ip_conn structure is used for identifying a connection. All
 * but one field in the structure are to be considered read-only by an
 * application. The only exception is the appstate field whos purpose
 * is to let the application store application-specific state (e.g.,
 * file pointers) for the connection. The type of this field is
 * configured in the "uipopt.h" header file.
 */
struct ip_conn
{
   IP_address ripaddr; /**< The IP address of the remote host. */

   uint16_t lport; /**< The local TCP port, in network byte order. */
   uint16_t rport; /**< The local remote TCP port, in network byte
			 order. */

   uint8_t rcv_nxt[4];    /**< The sequence number that we expect to
			 receive next. */
   uint8_t snd_nxt[4];    /**< The sequence number that was last sent by
                         us. */
   uint16_t len;          /**< Length of the data that was previously sent. */
   uint16_t mss;          /**< Current maximum segment size for the
			 connection. */
   uint16_t initialmss;   /**< Initial maximum segment size for the
			 connection. */
   uint8_t sa;            /**< Retransmission time-out calculation state
			 variable. */
   uint8_t sv;            /**< Retransmission time-out calculation state
			 variable. */
   uint8_t rto;           /**< Retransmission time-out. */
   uint8_t tcpstateflags; /**< TCP state and flags. */
   uint8_t timer;         /**< The retransmission timer. */
   uint8_t nrtx;          /**< The number of retransmissions for the last
			 segment sent. */

   /** The application state. */
   ip_tcp_appstate_t appstate;
};

/**
 * Pointer to the current TCP connection.
 *
 * The ip_conn pointer can be used to access the current TCP
 * connection.
 */
extern struct ip_conn *ip_conn;
/* The array containing all uIP connections. */
extern struct ip_conn ip_conns[IP_CONNS];

/**
 * 4-byte array used for the 32-bit sequence number calculations.
 */
extern uint8_t ip_acc32[4];

#if IP_UDP
/**
 * Representation of a uIP UDP connection.
 */
struct ip_udp_conn
{
   IP_address ripaddr; /**< The IP address of the remote peer. */
   uint16_t lport;   /**< The local port number in network byte order. */
   uint16_t rport;   /**< The remote port number in network byte order. */
   uint8_t ttl;      /**< Default time-to-live. */

   /** The application state. */
   ip_udp_appstate_t appstate;
};

/**
 * The current UDP connection.
 */
extern struct ip_udp_conn *ip_udp_conn;
extern struct ip_udp_conn ip_udp_conns[IP_UDP_CONNS];
#endif /* IP_UDP */

/**
 * The structure holding the TCP/IP statistics that are gathered if
 * IP_STATISTICS is set to 1.
 *
 */
struct ip_stats
{
   struct
   {
      ip_stats_t drop;     /**< Number of dropped packets at the IP
			     layer. */
      ip_stats_t recv;     /**< Number of received packets at the IP
			     layer. */
      ip_stats_t sent;     /**< Number of sent packets at the IP
			     layer. */
      ip_stats_t vhlerr;   /**< Number of packets dropped due to wrong
			     IP version or header length. */
      ip_stats_t hblenerr; /**< Number of packets dropped due to wrong
			     IP length, high byte. */
      ip_stats_t lblenerr; /**< Number of packets dropped due to wrong
			     IP length, low byte. */
      ip_stats_t fragerr;  /**< Number of packets dropped since they
			     were IP fragments. */
      ip_stats_t chkerr;   /**< Number of packets dropped due to IP
			     checksum errors. */
      ip_stats_t protoerr; /**< Number of packets dropped since they
			     were neither ICMP, UDP nor TCP. */
   } ip;                   /**< IP statistics. */
   struct
   {
      ip_stats_t drop;    /**< Number of dropped ICMP packets. */
      ip_stats_t recv;    /**< Number of received ICMP packets. */
      ip_stats_t sent;    /**< Number of sent ICMP packets. */
      ip_stats_t typeerr; /**< Number of ICMP packets with a wrong
			     type. */
   } icmp;                /**< ICMP statistics. */
   struct
   {
      ip_stats_t drop;    /**< Number of dropped TCP segments. */
      ip_stats_t recv;    /**< Number of recived TCP segments. */
      ip_stats_t sent;    /**< Number of sent TCP segments. */
      ip_stats_t chkerr;  /**< Number of TCP segments with a bad
			     checksum. */
      ip_stats_t ackerr;  /**< Number of TCP segments with a bad ACK
			     number. */
      ip_stats_t rst;     /**< Number of recevied TCP RST (reset) segments. */
      ip_stats_t rexmit;  /**< Number of retransmitted TCP segments. */
      ip_stats_t syndrop; /**< Number of dropped SYNs due to too few
			     connections was avaliable. */
      ip_stats_t synrst;  /**< Number of SYNs for closed ports,
			     triggering a RST. */
   } tcp;                 /**< TCP statistics. */
#if IP_UDP
   struct
   {
      ip_stats_t drop;   /**< Number of dropped UDP segments. */
      ip_stats_t recv;   /**< Number of recived UDP segments. */
      ip_stats_t sent;   /**< Number of sent UDP segments. */
      ip_stats_t chkerr; /**< Number of UDP segments with a bad
			     checksum. */
   } udp;                /**< UDP statistics. */
#endif                   /* IP_UDP */
};

/**
 * The uIP TCP/IP statistics.
 *
 * This is the variable in which the uIP TCP/IP statistics are gathered.
 */
extern struct ip_stats ip_stat;

/*---------------------------------------------------------------------------*/
/* All the stuff below this point is internal to uIP and should not be
 * used directly by an application or by a device driver.
 */
/*---------------------------------------------------------------------------*/
/* uint8_t ip_flags:
 *
 * When the application is called, ip_flags will contain the flags
 * that are defined in this file. Please read below for more
 * infomation.
 */
extern uint8_t ip_flags;

/* The following flags may be set in the global variable ip_flags
   before calling the application callback. The IP_ACKDATA,
   IP_NEWDATA, and IP_CLOSE flags may both be set at the same time,
   whereas the others are mutualy exclusive. Note that these flags
   should *NOT* be accessed directly, but only through the uIP
   functions/macros. */

#define IP_ACKDATA 1    /* Signifies that the outstanding data was \
             acked and the application should send                 \
             out new data instead of retransmitting                \
             the last data. */
#define IP_NEWDATA 2    /* Flags the fact that the peer has sent \
             us new data. */
#define IP_REXMIT 4     /* Tells the application to retransmit the \
             data that was last sent. */
#define IP_POLL 8       /* Used for polling the application, to \
             check if the application has data that             \
             it wants to send. */
#define IP_CLOSE 16     /* The remote host has closed the \
             connection, thus the connection has          \
             gone away. Or the application signals        \
             that it wants to close the                   \
             connection. */
#define IP_ABORT 32     /* The remote host has aborted the \
             connection, thus the connection has           \
             gone away. Or the application signals         \
             that it wants to abort the                    \
             connection. */
#define IP_CONNECTED 64 /* We have got a connection from a remote \
                            host and have set up a new connection \
                            for it, or an active connection has   \
                            been successfully established. */

#define IP_TIMEDOUT 128 /* The connection has been aborted due to \
             too many retransmissions. */

/* ip_process(flag):
 *
 * The actual uIP function which does all the work.
 */
void ip_process(uint8_t flag);

/* The following flags are passed as an argument to the ip_process()
   function. They are used to distinguish between the two cases where
   ip_process() is called. It can be called either because we have
   incoming data that should be processed, or because the periodic
   timer has fired. These values are never used directly, but only in
   the macrose defined in this file. */

#define IP_DATA 1          /* Tells uIP that there is incoming \
           data in the ip_buf buffer. The                      \
           length of the data is stored in the                 \
           global variable ip_len. */
#define IP_TIMER 2         /* Tells uIP that the periodic timer \
           has fired. */
#define IP_POLL_REQUEST 3  /* Tells uIP that a connection should \
           be polled. */
#define IP_UDP_SEND_CONN 4 /* Tells uIP that a UDP datagram \
           should be constructed in the                     \
           ip_buf buffer. */
#if IP_UDP
#define IP_UDP_TIMER 5
#endif /* IP_UDP */

/* The TCP states used in the ip_conn->tcpstateflags. */
#define IP_CLOSED 0
#define IP_SYN_RCVD 1
#define IP_SYN_SENT 2
#define IP_ESTABLISHED 3
#define IP_FIN_WAIT_1 4
#define IP_FIN_WAIT_2 5
#define IP_CLOSING 6
#define IP_TIME_WAIT 7
#define IP_LAST_ACK 8
#define IP_TS_MASK 15

#define IP_STOPPED 16

/* The TCP and IP headers. */
struct ip_tcpip_hdr
{
#if IP_CONF_IPV6
   /* IPv6 header. */
   uint8_t vtc,
       tcflow;
   uint16_t flow;
   uint8_t len[2];
   uint8_t proto, ttl;
   ip_ip6addr_t srcipaddr, destipaddr;
#else  /* IP_CONF_IPV6 */
   /* IPv4 header. */
   uint8_t vhl,
       tos,
       len[2],
       ipid[2],
       ipoffset[2],
       ttl,
       proto;
   uint16_t ipchksum;
   uint16_t srcipaddr[2],
       destipaddr[2];
#endif /* IP_CONF_IPV6 */

   /* TCP header. */
   uint16_t srcport,
       destport;
   uint8_t seqno[4],
       ackno[4],
       tcpoffset,
       flags,
       wnd[2];
   uint16_t tcpchksum;
   uint8_t urgp[2];
   uint8_t optdata[4];
};

/* The ICMP and IP headers. */
struct ip_icmpip_hdr
{
#if IP_CONF_IPV6
   /* IPv6 header. */
   uint8_t vtc,
       tcf;
   uint16_t flow;
   uint8_t len[2];
   uint8_t proto, ttl;
   ip_ip6addr_t srcipaddr, destipaddr;
#else  /* IP_CONF_IPV6 */
   /* IPv4 header. */
   uint8_t vhl,
       tos,
       len[2],
       ipid[2],
       ipoffset[2],
       ttl,
       proto;
   uint16_t ipchksum;
   uint16_t srcipaddr[2],
       destipaddr[2];
#endif /* IP_CONF_IPV6 */

   /* ICMP (echo) header. */
   uint8_t type, icode;
   uint16_t icmpchksum;
#if !IP_CONF_IPV6
   uint16_t id, seqno;
#else  /* !IP_CONF_IPV6 */
   uint8_t flags, reserved1, reserved2, reserved3;
   uint8_t icmp6data[16];
   uint8_t options[1];
#endif /* !IP_CONF_IPV6 */
};

/* The UDP and IP headers. */
struct ip_udpip_hdr
{
#if IP_CONF_IPV6
   /* IPv6 header. */
   uint8_t vtc,
       tcf;
   uint16_t flow;
   uint8_t len[2];
   uint8_t proto, ttl;
   ip_ip6addr_t srcipaddr, destipaddr;
#else  /* IP_CONF_IPV6 */
   /* IP header. */
   uint8_t vhl,
       tos,
       len[2],
       ipid[2],
       ipoffset[2],
       ttl,
       proto;
   uint16_t ipchksum;
   uint16_t srcipaddr[2],
       destipaddr[2];
#endif /* IP_CONF_IPV6 */

   /* UDP header. */
   uint16_t srcport,
       destport;
   uint16_t udplen;
   uint16_t udpchksum;
};

/**
 * The buffer size available for user data in the \ref ip_buf buffer.
 *
 * This macro holds the available size for user data in the \ref
 * ip_buf buffer. The macro is intended to be used for checking
 * bounds of available user data.
 *
 * Example:
 \code
 snprintf(ip_appdata, IP_APPDATA_SIZE, "%u\n", i);
 \endcode
 *
 * \hideinitializer
 */
#define IP_APPDATA_SIZE (IP_BUFSIZE - IP_LLH_LEN - IP_TCPIP_HLEN)

#define IP_PROTO_ICMP   1
#define IP_PROTO_TCP    6
#define IP_PROTO_UDP    17
#define IP_PROTO_ICMP6  58

/* Header sizes. */
#if IP_CONF_IPV6
#define IP_IPH_LEN 40
#else                                            /* IP_CONF_IPV6 */
#define IP_IPH_LEN 20                            /* Size of IP header */
#endif                                           /* IP_CONF_IPV6 */
#define IP_UDPH_LEN 8                            /* Size of UDP header */
#define IP_TCPH_LEN 20                           /* Size of TCP header */
#define IP_IPUDPH_LEN (IP_UDPH_LEN + IP_IPH_LEN) /* Size of IP + UDP header */
#define IP_IPTCPH_LEN (IP_TCPH_LEN + IP_IPH_LEN) /* Size of IP + TCP header */
#define IP_TCPIP_HLEN IP_IPTCPH_LEN

#if IP_FIXEDADDR
extern const IP_address ip_hostaddr, ip_netmask, ip_draddr;
#else  /* IP_FIXEDADDR */
extern IP_address ip_hostaddr, ip_netmask, ip_draddr;
#endif /* IP_FIXEDADDR */

/**
 * Representation of a 48-bit Ethernet address.
 */
struct ip_eth_addr
{
   uint8_t addr[6];
};

/**
 * Calculate the Internet checksum over a buffer.
 *
 * The Internet checksum is the one's complement of the one's
 * complement sum of all 16-bit words in the buffer.
 *
 * See RFC1071.
 *
 * \param buf A pointer to the buffer over which the checksum is to be
 * computed.
 *
 * \param len The length of the buffer over which the checksum is to
 * be computed.
 *
 * \return The Internet checksum of the buffer.
 */
uint16_t ip_chksum(uint16_t *buf, uint16_t len);

/**
 * Calculate the IP header checksum of the packet header in ip_buf.
 *
 * The IP header checksum is the Internet checksum of the 20 bytes of
 * the IP header.
 *
 * \return The IP header checksum of the IP header in the ip_buf
 * buffer.
 */
uint16_t ip_ipchksum(void);

/**
 * Calculate the TCP checksum of the packet in ip_buf and ip_appdata.
 *
 * The TCP checksum is the Internet checksum of data contents of the
 * TCP segment, and a pseudo-header as defined in RFC793.
 *
 * \return The TCP checksum of the TCP segment in ip_buf and pointed
 * to by ip_appdata.
 */
uint16_t ip_tcpchksum(void);

/**
 * Calculate the UDP checksum of the packet in ip_buf and ip_appdata.
 *
 * The UDP checksum is the Internet checksum of data contents of the
 * UDP segment, and a pseudo-header as defined in RFC768.
 *
 * \return The UDP checksum of the UDP segment in ip_buf and pointed
 * to by ip_appdata.
 */
uint16_t ip_udpchksum(void);

#endif /*IP_H*/