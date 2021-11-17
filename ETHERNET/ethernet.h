/**
 * @file ethernet.h
 * @author José Roberto Parra Trewartha (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdbool.h>
#include <stdint.h>
#include "dhcp.h"
#include "utilities/enc28j60.h"
#include "ethernet_client.h"
#include "ethernet_server.h"
#include "ethernet_udp.h"
#include "utilities/ip-conf.h"
#include "utilities/ip.h"
#include "utilities/ip_arp.h"

#define IPETHERNET_FREEPACKET 1
#define IPETHERNET_SENDPACKET 2
#define IPETHERNET_BUFFERREAD 4

#define ip_ip_addr(addr, ip)                                   \
	do                                                          \
	{                                                           \
		((u16_t *)(addr))[0] = HTONS(((ip[0]) << 8) | (ip[1])); \
		((u16_t *)(addr))[1] = HTONS(((ip[2]) << 8) | (ip[3])); \
	} while (0)

#define ip_addr_ip(a) IP_address(a[0] & 0xFF, a[0] >> 8, a[1] & 0xFF, a[1] >> 8) //TODO this is not IPV6 capable

#define ip_seteth_addr(eaddr)          \
	do                                  \
	{                                   \
		ip_ethaddr.addr[0] = eaddr[0]; \
		ip_ethaddr.addr[1] = eaddr[1]; \
		ip_ethaddr.addr[2] = eaddr[2]; \
		ip_ethaddr.addr[3] = eaddr[3]; \
		ip_ethaddr.addr[4] = eaddr[4]; \
		ip_ethaddr.addr[5] = eaddr[5]; \
	} while (0)

#define BUF ((struct ip_tcpip_hdr *)&ip_buf[IP_LLH_LEN])

enum EthernetLinkStatus {
	Unknown,
	LinkON,
	LinkOFF
};

enum EthernetHardwareStatus {
	EthernetNoHardware,
	EthernetW5100,
	EthernetW5200,
	EthernetW5500,
	EthernetENC28J60 = 10
};

/**
 * @brief 
 * 
 */
typedef struct ip_ethernet {
	bool initialized;
	memhandle in_packet;
	memhandle ip_packet;
	uint8_t ip_hdrlen;
	uint8_t packetstate;

	IP_address _dnsServerAddress;
	Dhcp_t _dhcp;

	uint32_t periodic_timer;
} Ethernet;

void ip_ethernet_init(Ethernet *eth, const uint8_t *mac);
void ip_ethernet_configure(Ethernet *eth, IP_address ip, IP_address dns, IP_address gateway, IP_address subnet);
void Ethernetick(Ethernet *eth);

bool ip_ethernet_network_send(Ethernet *eth);

void ip_ethernet_call_yield(Ethernet *eth);

#if IP_UDP
uint16_t ip_ethernet_upper_layer_chksum(Ethernet *eth, uint8_t proto);
#endif
   
#if IP_CONF_IPV6
uint16_t ip_icmp6chksum(Ethernet *eth);
#endif /* IP_CONF_IPV6 */

uint16_t ip_ethernet_chksum(Ethernet *eth, uint16_t sum, const uint8_t* data, uint16_t len);
uint16_t ip_ethernet_ipchksum(Ethernet *eth);


#endif /*ETHERNET_H*/