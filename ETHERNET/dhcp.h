#ifndef DHCP_H
#define DHCP_H

#include "ethernet_udp.h"
#include "utilities/ip.h"
#include "../INTERNET/MAC/MAC.h"
#include <stdint.h>

/* DHCP state machine. */
#define STATE_DHCP_START 0
#define STATE_DHCP_DISCOVER 1
#define STATE_DHCP_REQUEST 2
#define STATE_DHCP_LEASED 3
#define STATE_DHCP_REREQUEST 4
#define STATE_DHCP_RELEASE 5

#define DHCP_FLAGSBROADCAST 0x8000

/* UDP port numbers for DHCP */
#define DHCP_SERVER_PORT 67 /* from server to client */
#define DHCP_CLIENT_PORT 68 /* from client to server */

/* DHCP message OP code */
#define DHCP_BOOTREQUEST 1
#define DHCP_BOOTREPLY 2

/* DHCP message type */
#define DHCP_DISCOVER 1
#define DHCP_OFFER 2
#define DHCP_REQUEST 3
#define DHCP_DECLINE 4
#define DHCP_ACK 5
#define DHCP_NAK 6
#define DHCP_RELEASE 7
#define DHCP_INFORM 8

#define DHCP_HTYPE10MB 1
#define DHCP_HTYPE100MB 2

#define DHCP_HLENETHERNET 6
#define DHCP_HOPS 0
#define DHCP_SECS 0

#define MAGIC_COOKIE 0x63825363
#define MAX_DHCP_OPT 16

#define HOST_NAME "ENC28J"
#define DEFAULT_LEASE (900) //default lease time in seconds

#define DHCP_CHECK_NONE (0)
#define DHCP_CHECK_RENEW_FAIL (1)
#define DHCP_CHECK_RENEW_OK (2)
#define DHCP_CHECK_REBIND_FAIL (3)
#define DHCP_CHECK_REBIND_OK (4)

enum {
	padOption = 0,
	subnetMask = 1,
	timerOffset = 2,
	routersOnSubnet = 3,
	/* timeServer		=	4,
	nameServer		=	5,*/
	dns = 6,
	/*logServer		=	7,
	cookieServer		=	8,
	lprServer		=	9,
	impressServer		=	10,
	resourceLocationServer	=	11,*/
	hostName = 12,
	/*bootFileSize		=	13,
	meritDumpFile		=	14,*/
	domainName = 15,
	/*swapServer		=	16,
	rootPath		=	17,
	extentionsPath		=	18,
	IPforwarding		=	19,
	nonLocalSourceRouting	=	20,
	policyFilter		=	21,
	maxDgramReasmSize	=	22,
	defaultIPTTL		=	23,
	pathMTUagingTimeout	=	24,
	pathMTUplateauTable	=	25,
	ifMTU			=	26,
	allSubnetsLocal		=	27,
	broadcastAddr		=	28,
	performMaskDiscovery	=	29,
	maskSupplier		=	30,
	performRouterDiscovery	=	31,
	routerSolicitationAddr	=	32,
	staticRoute		=	33,
	trailerEncapsulation	=	34,
	arpCacheTimeout		=	35,
	ethernetEncapsulation	=	36,
	tcpDefaultTTL		=	37,
	tcpKeepaliveInterval	=	38,
	tcpKeepaliveGarbage	=	39,
	nisDomainName		=	40,
	nisServers		=	41,
	ntpServers		=	42,
	vendorSpecificInfo	=	43,
	netBIOSnameServer	=	44,
	netBIOSdgramDistServer	=	45,
	netBIOSnodeType		=	46,
	netBIOSscope		=	47,
	xFontServer		=	48,
	xDisplayManager		=	49,*/
	dhcpRequestedIPaddr = 50,
	dhcpIPaddrLeaseTime = 51,
	/*dhcpOptionOverload	=	52,*/
	dhcpMessageType = 53,
	dhcpServerIdentifier = 54,
	dhcpParamRequest = 55,
	/*dhcpMsg			=	56,
	dhcpMaxMsgSize		=	57,*/
	dhcpT1value = 58,
	dhcpT2value = 59,
	/*dhcpClassIdentifier	=	60,*/
	dhcpClientIdentifier = 61,
	endOption = 255
};

typedef struct _RIP_MSG_FIXED {
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t flags;
	uint8_t ciaddr[4];
	uint8_t yiaddr[4];
	uint8_t siaddr[4];
	uint8_t giaddr[4];
	uint8_t chaddr[6];
} RIP_MSG_FIXED;

typedef struct _Dhcp_t {
	uint32_t _InitialTransactionId;
	uint32_t _TransactionId;
	MAC_address_t _MacAddr;	
	
	IP_address _LocalIp;
	IP_address _SubnetMask;
	IP_address _GatewayIp;
	IP_address _DhcpServerIp;
	IP_address _DnsServerIp;

	uint32_t _LeaseTime;
	uint32_t _T1, _T2;
	int32_t _renewInSec;
	int32_t _rebindInSec;
	int32_t _lastCheck;
	uint32_t _timeout;
	uint32_t _responseTimeout;
	uint32_t _secTimeout;
	uint8_t _state;
	EthernetUDP _UdpSocket;
} Dhcp_t;

// Funciones privadas
int16_t request_DHCP_lease(Dhcp_t *dhcp);
void reset_DHCP_lease(Dhcp_t *dhcp);
void presend_DHCP(Dhcp_t *dhcp);
void send_DHCP_MESSAGE(Dhcp_t *dhcp,uint8_t messageType, uint16_t secondsElapsed);
void printByte(Dhcp_t *dhcp,char *, uint8_t);
uint8_t parseDHCPResponse(Dhcp_t *dhcp, uint32_t responseTimeout, uint32_t *transactionId);

// Funciones publicas
IP_address getLocalIp(Dhcp_t *dhcp);
IP_address getSubnetMask(Dhcp_t *dhcp);
IP_address getGatewayIp(Dhcp_t *dhcp);
IP_address getDhcpServerIp(Dhcp_t *dhcp);
IP_address getDnsServerIp(Dhcp_t *dhcp);

int16_t beginWithDHCP(Dhcp_t *dhcp,MAC_address_t *mac, uint32_t timeout, uint32_t responseTimeout);
int16_t checkLease(Dhcp_t *dhcp);

#endif /*DHCP_H*/