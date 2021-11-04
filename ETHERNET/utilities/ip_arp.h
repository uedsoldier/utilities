#ifndef __IP_ARP_H__
#define __IP_ARP_H__

#include "ip.h"


extern struct ip_eth_addr ip_ethaddr;

/**
 * @brief  The Ethernet header.
 */
struct ip_eth_hdr {
  struct ip_eth_addr dest;
  struct ip_eth_addr src;
  uint16_t type;
};

#define IP_ETHTYPE_ARP 0x0806
#define IP_ETHTYPE_IP  0x0800
#define IP_ETHTYPE_IP6 0x86dd


/* The ip_arp_init() function must be called before any of the other
   ARP functions. */
void ip_arp_init(void);

/* The ip_arp_ipin() function should be called whenever an IP packet
   arrives from the Ethernet. This function refreshes the ARP table or
   inserts a new mapping if none exists. The function assumes that an
   IP packet with an Ethernet header is present in the ip_buf buffer
   and that the length of the packet is in the ip_len variable. */
void ip_arp_ipin(void);
//#define ip_arp_ipin()

/* The ip_arp_arpin() should be called when an ARP packet is received
   by the Ethernet driver. This function also assumes that the
   Ethernet frame is present in the ip_buf buffer. When the
   ip_arp_arpin() function returns, the contents of the ip_buf
   buffer should be sent out on the Ethernet if the ip_len variable
   is > 0. */
void ip_arp_arpin(void);

/* The ip_arp_out() function should be called when an IP packet
   should be sent out on the Ethernet. This function creates an
   Ethernet header before the IP header in the ip_buf buffer. The
   Ethernet header will have the correct Ethernet MAC destination
   address filled in if an ARP table entry for the destination IP
   address (or the IP address of the default router) is present. If no
   such table entry is found, the IP packet is overwritten with an ARP
   request and we rely on TCP to retransmit the packet that was
   overwritten. In any case, the ip_len variable holds the length of
   the Ethernet frame that should be transmitted. */
void ip_arp_out(void);

/* The ip_arp_timer() function should be called every ten seconds. It
   is responsible for flushing old entries in the ARP table. */
void ip_arp_timer(void);


/**
 * Specifiy the Ethernet MAC address.
 *
 * The ARP code needs to know the MAC address of the Ethernet card in
 * order to be able to respond to ARP queries and to generate working
 * Ethernet headers.
 *
 * \note This macro only specifies the Ethernet MAC address to the ARP
 * code. It cannot be used to change the MAC address of the Ethernet
 * card.
 *
 * \param eaddr A pointer to a struct ip_eth_addr containing the
 * Ethernet MAC address of the Ethernet card.
 *
 * \hideinitializer
 */
#define ip_setethaddr(eaddr) do {ip_ethaddr.addr[0] = eaddr.addr[0]; \
                              ip_ethaddr.addr[1] = eaddr.addr[1];\
                              ip_ethaddr.addr[2] = eaddr.addr[2];\
                              ip_ethaddr.addr[3] = eaddr.addr[3];\
                              ip_ethaddr.addr[4] = eaddr.addr[4];\
                              ip_ethaddr.addr[5] = eaddr.addr[5];} while(0)


#endif /* __IP_ARP_H__ */