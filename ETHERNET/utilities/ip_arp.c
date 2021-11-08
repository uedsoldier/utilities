/**
 * IP Address Resolution Protocol
 *
 * The Address Resolution Protocol ARP is used for mapping between IP
 * addresses and link level addresses such as the Ethernet MAC
 * addresses. ARP uses broadcast queries to ask for the link level
 * address of a known IP address and the host which is configured with
 * the IP address for which the query was meant, will respond with its
 * link level address.
 *
 * 
 * @note This ARP implementation only supports Ethernet.
 */

#include "ip_arp.h"

#include <string.h>

struct arp_hdr {
	struct ip_eth_hdr ethhdr;
	uint16_t hwtype;
	uint16_t protocol;
	uint8_t hwlen;
	uint8_t protolen;
	uint16_t opcode;
	struct ip_eth_addr shwaddr;
	uint16_t sipaddr[2];
	struct ip_eth_addr dhwaddr;
	uint16_t dipaddr[2];
};

struct ethip_hdr {
	struct ip_eth_hdr ethhdr;
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
};

#define ARP_REQUEST 1
#define ARP_REPLY 2

#define ARP_HWTYPE_ETH 1

struct arp_entry {
	uint16_t ipaddr[2];
	struct ip_eth_addr ethaddr;
	uint8_t time;
};

static const struct ip_eth_addr broadcast_ethaddr = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
static const uint16_t broadcast_ipaddr[2] = {0xFFFF, 0xFFFF};

static struct arp_entry arp_table[IP_ARPTAB_SIZE];
static uint16_t ipaddr[2];
static uint8_t i, c;

static uint8_t arptime;
static uint8_t tmpage;

#define BUF ((struct arp_hdr *)&ip_buf[0])
#define IPBUF ((struct ethip_hdr *)&ip_buf[0])
/*-----------------------------------------------------------------------------------*/
/**
 * Initialize the ARP module.
 *
 */
/*-----------------------------------------------------------------------------------*/
void ip_arp_init(void) {
	for (i = 0; i < IP_ARPTAB_SIZE; ++i) {
		memset(arp_table[i].ipaddr, 0, 4);
	}
}
/*-----------------------------------------------------------------------------------*/
/**
 * Periodic ARP processing function.
 *
 * This function performs periodic timer processing in the ARP module
 * and should be called at regular intervals. The recommended interval
 * is 10 seconds between the calls.
 *
 */
/*-----------------------------------------------------------------------------------*/
void ip_arp_timer(void) {
	struct arp_entry *tabptr;

	++arptime;
	for (i = 0; i < IP_ARPTAB_SIZE; ++i) {
		tabptr = &arp_table[i];
		if ((tabptr->ipaddr[0] | tabptr->ipaddr[1]) != 0 && arptime - tabptr->time >= IP_ARP_MAXAGE) {
			memset(tabptr->ipaddr, 0, 4);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
static void
ip_arp_update(uint16_t *ipaddr, struct ip_eth_addr *ethaddr) {
	register struct arp_entry *tabptr;
	/* Walk through the ARP mapping table and try to find an entry to
     update. If none is found, the IP -> MAC address mapping is
     inserted in the ARP table. */
	for (i = 0; i < IP_ARPTAB_SIZE; ++i) {

		tabptr = &arp_table[i];
		/* Only check those entries that are actually in use. */
		if (tabptr->ipaddr[0] != 0 && tabptr->ipaddr[1] != 0) {

			/* Check if the source IP address of the incoming packet matches
         the IP address in this ARP table entry. */
			if (ipaddr[0] == tabptr->ipaddr[0] && ipaddr[1] == tabptr->ipaddr[1]) {
				/* An old entry found, update this and return. */
				memcpy(tabptr->ethaddr.addr, ethaddr->addr, 6);
				tabptr->time = arptime;
				return;
			}
		}
	}

	/* If we get here, no existing ARP table entry was found, so we
     create one. */

	/* First, we try to find an unused entry in the ARP table. */
	for (i = 0; i < IP_ARPTAB_SIZE; ++i) {
		tabptr = &arp_table[i];
		if (tabptr->ipaddr[0] == 0 && tabptr->ipaddr[1] == 0) {
			break;
		}
	}

	/* If no unused entry is found, we try to find the oldest entry and
     throw it away. */
	if (i == IP_ARPTAB_SIZE) {
		tmpage = 0;
		c = 0;
		for (i = 0; i < IP_ARPTAB_SIZE; ++i) {
			tabptr = &arp_table[i];
			if (arptime - tabptr->time > tmpage) {
				tmpage = arptime - tabptr->time;
				c = i;
			}
		}
		i = c;
		tabptr = &arp_table[i];
	}

	/* Now, i is the ARP table entry which we will fill with the new
     information. */
	memcpy(tabptr->ipaddr, ipaddr, 4);
	memcpy(tabptr->ethaddr.addr, ethaddr->addr, 6);
	tabptr->time = arptime;
}
/*-----------------------------------------------------------------------------------*/
/**
 * ARP processing for incoming IP packets
 *
 * This function should be called by the device driver when an IP
 * packet has been received. The function will check if the address is
 * in the ARP cache, and if so the ARP cache entry will be
 * refreshed. If no ARP cache entry was found, a new one is created.
 *
 * This function expects an IP packet with a prepended Ethernet header
 * in the ip_buf[] buffer, and the length of the packet in the global
 * variable ip_len.
 */
/*-----------------------------------------------------------------------------------*/
//#if 0
void ip_arp_ipin(void) {
	ip_len -= sizeof(struct ip_eth_hdr);

	/* Only insert/update an entry if the source IP address of the
     incoming IP packet comes from a host on the local network. */
	if ((IPBUF->srcipaddr[0] & ip_netmask[0]) != (ip_hostaddr[0] & ip_netmask[0])) {
		return;
	}
	if ((IPBUF->srcipaddr[1] & ip_netmask[1]) != (ip_hostaddr[1] & ip_netmask[1])) {
		return;
	}
	ip_arp_update(IPBUF->srcipaddr, &(IPBUF->ethhdr.src));

	return;
}
//#endif /* 0 */
/*-----------------------------------------------------------------------------------*/
/**
 * ARP processing for incoming ARP packets.
 *
 * This function should be called by the device driver when an ARP
 * packet has been received. The function will act differently
 * depending on the ARP packet type: if it is a reply for a request
 * that we previously sent out, the ARP cache will be filled in with
 * the values from the ARP reply. If the incoming ARP packet is an ARP
 * request for our IP address, an ARP reply packet is created and put
 * into the ip_buf[] buffer.
 *
 * When the function returns, the value of the global variable ip_len
 * indicates whether the device driver should send out a packet or
 * not. If ip_len is zero, no packet should be sent. If ip_len is
 * non-zero, it contains the length of the outbound packet that is
 * present in the ip_buf[] buffer.
 *
 * This function expects an ARP packet with a prepended Ethernet
 * header in the ip_buf[] buffer, and the length of the packet in the
 * global variable ip_len.
 */
/*-----------------------------------------------------------------------------------*/
void ip_arp_arpin(void) {
	if (ip_len < sizeof(struct arp_hdr))
	{
		ip_len = 0;
		return;
	}
	ip_len = 0;

	switch (BUF->opcode) {
	case HTONS(ARP_REQUEST):
		/* ARP request. If it asked for our address, we send out a
       reply. */
		if (ip_ipaddr_cmp(BUF->dipaddr, ip_hostaddr)) {
			/* First, we register the one who made the request in our ARP
	 table, since it is likely that we will do more communication
	 with this host in the future. */
			ip_arp_update(BUF->sipaddr, &BUF->shwaddr);

			/* The reply opcode is 2. */
			BUF->opcode = HTONS(2);

			memcpy(BUF->dhwaddr.addr, BUF->shwaddr.addr, 6);
			memcpy(BUF->shwaddr.addr, ip_ethaddr.addr, 6);
			memcpy(BUF->ethhdr.src.addr, ip_ethaddr.addr, 6);
			memcpy(BUF->ethhdr.dest.addr, BUF->dhwaddr.addr, 6);

			BUF->dipaddr[0] = BUF->sipaddr[0];
			BUF->dipaddr[1] = BUF->sipaddr[1];
			BUF->sipaddr[0] = ip_hostaddr[0];
			BUF->sipaddr[1] = ip_hostaddr[1];

			BUF->ethhdr.type = HTONS(IP_ETHTYPE_ARP);
			ip_len = sizeof(struct arp_hdr);
		}
		break;
	case HTONS(ARP_REPLY):
		/* ARP reply. We insert or update the ARP table if it was meant
       for us. */
		if (ip_ipaddr_cmp(BUF->dipaddr, ip_hostaddr)) {
			ip_arp_update(BUF->sipaddr, &BUF->shwaddr);
		}
		break;
	}

	return;
}
/*-----------------------------------------------------------------------------------*/
/**
 * Prepend Ethernet header to an outbound IP packet and see if we need
 * to send out an ARP request.
 *
 * This function should be called before sending out an IP packet. The
 * function checks the destination IP address of the IP packet to see
 * what Ethernet MAC address that should be used as a destination MAC
 * address on the Ethernet.
 *
 * If the destination IP address is in the local network (determined
 * by logical ANDing of netmask and our IP address), the function
 * checks the ARP cache to see if an entry for the destination IP
 * address is found. If so, an Ethernet header is prepended and the
 * function returns. If no ARP cache entry is found for the
 * destination IP address, the packet in the ip_buf[] is replaced by
 * an ARP request packet for the IP address. The IP packet is dropped
 * and it is assumed that they higher level protocols (e.g., TCP)
 * eventually will retransmit the dropped packet.
 *
 * If the destination IP address is not on the local network, the IP
 * address of the default router is used instead.
 *
 * When the function returns, a packet is present in the ip_buf[]
 * buffer, and the length of the packet is in the global variable
 * ip_len.
 */
/*-----------------------------------------------------------------------------------*/
void ip_arp_out(void) {
	struct arp_entry *tabptr;

	/* Find the destination IP address in the ARP table and construct
     the Ethernet header. If the destination IP addres isn't on the
     local network, we use the default router's IP address instead.
     If not ARP table entry is found, we overwrite the original IP
     packet with an ARP request for the IP address. */

	/* First check if destination is a local broadcast. */
	if (ip_ipaddr_cmp(IPBUF->destipaddr, broadcast_ipaddr)) {
		memcpy(IPBUF->ethhdr.dest.addr, broadcast_ethaddr.addr, 6);
	}
	else {
		/* Check if the destination address is on the local network. */
		if (!ip_ipaddr_maskcmp(IPBUF->destipaddr, ip_hostaddr, ip_netmask)) {
			/* Destination address was not on the local network, so we need to
	 use the default router's IP address instead of the destination
	 address when determining the MAC address. */
			ip_ipaddr_copy(ipaddr, ip_draddr);
		}
		else {
			/* Else, we use the destination IP address. */
			ip_ipaddr_copy(ipaddr, IPBUF->destipaddr);
		}

		for (i = 0; i < IP_ARPTAB_SIZE; ++i) {
			tabptr = &arp_table[i];
			if (ip_ipaddr_cmp(ipaddr, tabptr->ipaddr)) {
				break;
			}
		}

		if (i == IP_ARPTAB_SIZE) {
			/* The destination address was not in our ARP table, so we
	 overwrite the IP packet with an ARP request. */

			memset(BUF->ethhdr.dest.addr, 0xFF, 6);
			memset(BUF->dhwaddr.addr, 0x00, 6);
			memcpy(BUF->ethhdr.src.addr, ip_ethaddr.addr, 6);
			memcpy(BUF->shwaddr.addr, ip_ethaddr.addr, 6);

			ip_ipaddr_copy(BUF->dipaddr, ipaddr);
			ip_ipaddr_copy(BUF->sipaddr, ip_hostaddr);
			BUF->opcode = HTONS(ARP_REQUEST); /* ARP request. */
			BUF->hwtype = HTONS(ARP_HWTYPE_ETH);
			BUF->protocol = HTONS(IP_ETHTYPE_IP);
			BUF->hwlen = 6;
			BUF->protolen = 4;
			BUF->ethhdr.type = HTONS(IP_ETHTYPE_ARP);

			ip_appdata = &ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN];

			ip_len = sizeof(struct arp_hdr);
			return;
		}

		/* Build an ethernet header. */
		memcpy(IPBUF->ethhdr.dest.addr, tabptr->ethaddr.addr, 6);
	}
	memcpy(IPBUF->ethhdr.src.addr, ip_ethaddr.addr, 6);

	IPBUF->ethhdr.type = HTONS(IP_ETHTYPE_IP);

	ip_len += sizeof(struct ip_eth_hdr);
}