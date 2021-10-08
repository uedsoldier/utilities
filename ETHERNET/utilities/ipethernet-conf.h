#ifndef IPETHERNET_CONF_H
#define IPETHERNET_CONF_H


// TODO

/**
 * TCP
*/
#ifndef IP_SOCKET_NUMPACKETS
#define IP_SOCKET_NUMPACKETS    5
#endif
#ifndef IP_CONF_MAX_CONNECTIONS
#define IP_CONF_MAX_CONNECTIONS 4
#endif

/**
 * UDP
 * Set IP_CONF_UDP to 0 to disable UDP (saves aprox. 5kB flash)
*/
#ifndef IP_CONF_UDP
#define IP_CONF_UDP             1
#endif
#ifndef IP_CONF_BROADCAST
#define IP_CONF_BROADCAST       1
#endif
#ifndef IP_CONF_UDP_CONNS
#define IP_CONF_UDP_CONNS       4
#endif

/** 
 * Timeout in ms for attempts to get a free memory block to write
 * before returning number of bytes sent so far
 * set to 0 to block until connection is closed by timeout */
#ifndef IP_WRITE_TIMEOUT
#define IP_WRITE_TIMEOUT        0
#endif

/* timeout after which UIPClient::connect gives up. The timeout is specified in seconds.
 * if set to a number <= 0 connect will timeout when uIP does (which might be longer than you expect...) */
#ifndef IP_CONNECT_TIMEOUT
#define IP_CONNECT_TIMEOUT      -1
#endif

/* periodic timer for uip (in ms) */
#ifndef IP_PERIODIC_TIMER
#define IP_PERIODIC_TIMER       250
#endif

/* timer to poll client for data after last write (in ms)
 * set to -1 to disable fast polling and rely on periodic only (saves 100 bytes flash) */
#ifndef IP_CLIENT_TIMER
#define IP_CLIENT_TIMER         10
#endif


#endif /*IPETHERNET_CONF_H*/