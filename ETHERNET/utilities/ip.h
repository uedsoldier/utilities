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
typedef IPV6_address_t IP_address_t 
#else /* IP_CONF_IPV6 */
typedef IPV4_address_t IP_address_t 
#endif


/**
 * @brief Set the IP address of this host.
 * The IP address is represented as a 4-byte array where the first
 * octet of the IP address is put in the first member of the 4-byte
 * array.
 *
 * Example:
    \code
    IP_address_t addr;
    ip_ipaddr(&addr, 192,168,1,2);
    ip_sethostaddr(&addr);

    \endcode
 * @param addr A pointer to an IP address of type IP_address_t;
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
    IP_address_t hostaddr;
    ip_gethostaddr(&hostaddr);
    \endcode
 * @param addr A pointer to a IP_address_t variable that will be
 * filled in with the currently configured IP address.
 */
#define ip_gethostaddr(addr) ip_ipaddr_copy((addr), ip_hostaddr)

/**
 * @brief Set the default router's IP address.
 *
 * @param addr A pointer to a IP_address_t variable containing the IP
 * address of the default router.
 */
#define ip_setdraddr(addr) ip_ipaddr_copy(ip_draddr, (addr))

/**
 * @brief Set the netmask.
 * @param addr A pointer to a IP_address_t variable containing the IP
 * address of the netmask.
 */
#define ip_setnetmask(addr) ip_ipaddr_copy(ip_netmask, (addr))

/**
 * @brief Get the default router's IP address.
 * @param addr A pointer to a IP_address_t variable that will be
 * filled in with the IP address of the default router.
 */
#define ip_getdraddr(addr) ip_ipaddr_copy((addr), ip_draddr)

/**
 * @brief Get the netmask.
 * @param addr A pointer to a IP_address_t variable that will be
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
#define ip_input()        ip_process(IP_DATA)

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
#define ip_periodic(conn) do { ip_conn = &ip_conns[conn]; ip_process(IP_TIMER); } while (0)

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
#define ip_periodic_conn(conn) do { ip_conn = conn; ip_process(IP_TIMER); } while (0)

/**
 * @brief Request that a particular connection should be polled.
 *
 * Similar to ip_periodic_conn() but does not perform any timer
 * processing. The application is polled for new data.
 *
 * @param conn A pointer to the ip_conn struct for the connection to
 * be processed.
 */
#define ip_poll_conn(conn) do { ip_conn = conn; ip_process(IP_POLL_REQUEST); } while (0)

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
#define ip_udp_periodic(conn) do { ip_udp_conn = &ip_udp_conns[conn]; ip_process(IP_UDP_TIMER); } while (0)

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
#define ip_udp_periodic_conn(conn) do { ip_udp_conn = conn; ip_process(IP_UDP_TIMER); } while (0)

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
extern uint8_t ip_buf[IP_BUFSIZE+2];

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
    IP_address_t ipaddr;
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
struct ip_conn *ip_connect(IP_address_t *ripaddr, uint16_t port);



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
#define ip_datalen()       ip_len

/**
 * The length of any out-of-band data (urgent data) that has arrived
 * on the connection.
 *
 * \note The configuration parameter IP_URGDATA must be set for this
 * function to be enabled.
 *

 */
#define ip_urgdatalen()    ip_urglen

/**
 * Close the current connection.
 *
 * This function will close the current connection in a nice way.
 *

 */
#define ip_close()         (ip_flags = IP_CLOSE)

/**
 * Abort the current connection.
 *
 * This function will abort (reset) the current connection, and is
 * usually used when an error has occured that prevents using the
 * ip_close() function.
 *

 */
#define ip_abort()         (ip_flags = IP_ABORT)

/**
 * Tell the sending host to stop sending data.
 *
 * This function will close our receiver's window so that we stop
 * receiving data for the current connection.
 *

 */
#define ip_stop()          (ip_conn->tcpstateflags |= IP_STOPPED)

/**
 * Find out if the current connection has been previously stopped with
 * ip_stop().
 *

 */
#define ip_stopped(conn)   ((conn)->tcpstateflags & IP_STOPPED)

/**
 * Restart the current connection, if is has previously been stopped
 * with ip_stop().
 *
 * This function will open the receiver's window again so that we
 * start receiving data for the current connection.
 *

 */
#define ip_restart()         do { ip_flags |= IP_NEWDATA; \
                                   ip_conn->tcpstateflags &= ~IP_STOPPED; \
                              } while(0)


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
#define ip_newdata()   (ip_flags & IP_NEWDATA)

/**
 * Has previously sent data been acknowledged?
 *
 * Will reduce to non-zero if the previously sent data has been
 * acknowledged by the remote host. This means that the application
 * can send new data.
 *

 */
#define ip_acked()   (ip_flags & IP_ACKDATA)

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
#define ip_closed()    (ip_flags & IP_CLOSE)

/**
 * Has the connection been aborted by the other end?
 *
 * Non-zero if the current connection has been aborted (reset) by the
 * remote host.
 *

 */
#define ip_aborted()    (ip_flags & IP_ABORT)

/**
 * Has the connection timed out?
 *
 * Non-zero if the current connection has been aborted due to too many
 * retransmissions.
 *

 */
#define ip_timedout()    (ip_flags & IP_TIMEDOUT)

/**
 * Do we need to retransmit previously data?
 *
 * Reduces to non-zero if the previously sent data has been lost in
 * the network, and the application should retransmit it. The
 * application should send the exact same data as it did the last
 * time, using the ip_send() function.
 *

 */
#define ip_rexmit()     (ip_flags & IP_REXMIT)

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
#define ip_poll()       (ip_flags & IP_POLL)

/**
 * Get the initial maxium segment size (MSS) of the current
 * connection.
 *

 */
#define ip_initialmss()             (ip_conn->initialmss)

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
#define ip_mss()             (ip_conn->mss)

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
    IP_address_t addr;
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
struct ip_udp_conn *ip_udp_new(IP_address_t *ripaddr, uint16_t rport);

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



#endif /*IP_H*/