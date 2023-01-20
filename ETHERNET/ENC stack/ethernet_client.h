/**
 * @file ethernet_client.h
 * @author Jose Roberto Parra Trewartha (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ETHERNET_CLIENT_H
#define ETHERNET_CLIENT_H

#include <stdint.h>
#include <stdbool.h>
#include "utilities/mempool.h"
#include "utilities/ip.h"
#include "utilities/ip-conf.h"
#include "utilities/ip_arp.h"

#include "ethernet.h"
#include "ethernet_client.h"
#include "dns.h"

#define IP_TCP_PHYH_LEN IP_LLH_LEN+IP_IPTCPH_LEN


#define IP_SOCKET_DATALEN IP_TCP_MSS

#define IP_CLIENT_CONNECTED 0x01
#define IP_CLIENT_CLOSE 0x02
#define IP_CLIENT_REMOTECLOSED 0x04
#define IP_CLIENT_RESTART 0x08
#define IP_CLIENT_ACCEPTED 0x10

typedef struct {
	uint8_t conn_index;
	uint8_t state;
	memhandle packets_in[IP_SOCKET_NUMPACKETS];
	uint16_t lport; /**< The local TCP port, in network byte order. */
} ip_userdata_closed_t;

typedef struct {
	uint8_t conn_index;
	uint8_t state;
	memhandle packets_in[IP_SOCKET_NUMPACKETS];
	memhandle packets_out[IP_SOCKET_NUMPACKETS];
	memaddress out_pos;
} ip_userdata_t;

/**
 * @brief 
 * 
 */
typedef struct {
	ip_userdata_t *data;
    ip_userdata_t all_data[IP_CONNS];
} Ethernet_client;

// Funciones
void EthernetClient_init(Ethernet_client *client);


#endif /* ETHERNET_CLIENT_H */
