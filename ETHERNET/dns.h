#ifndef DNS_H
#define DNS_H

#include "ethernet_udp.h"
#include "utilities/ip.h"
#include <stdint.h>

typedef struct _Dns{
    IP_address iDNSServer;
    uint16_t iRequestId;
    EthernetUDP iUdp;
} Dns;

#define SOCKET_NONE	255
// Various flags and header field values for a DNS message
#define UDP_HEADER_SIZE	8
#define DNS_HEADER_SIZE	12
#define TTL_SIZE        4
#define QUERY_FLAG               (0)
#define RESPONSE_FLAG            (1<<15)
#define QUERY_RESPONSE_MASK      (1<<15)
#define OPCODE_STANDARD_QUERY    (0)
#define OPCODE_INVERSE_QUERY     (1<<11)
#define OPCODE_STATUS_REQUEST    (2<<11)
#define OPCODE_MASK              (15<<11)
#define AUTHORITATIVE_FLAG       (1<<10)
#define TRUNCATION_FLAG          (1<<9)
#define RECURSION_DESIRED_FLAG   (1<<8)
#define RECURSION_AVAILABLE_FLAG (1<<7)
#define RESP_NO_ERROR            (0)
#define RESP_FORMAT_ERROR        (1)
#define RESP_SERVER_FAILURE      (2)
#define RESP_NAME_ERROR          (3)
#define RESP_NOT_IMPLEMENTED     (4)
#define RESP_REFUSED             (5)
#define RESP_MASK                (15)
#define TYPE_A                   (0x0001)
#define CLASS_IN                 (0x0001)
#define LABEL_COMPRESSION_MASK   (0xC0)
// Port number that DNS servers listen on
#define DNS_PORT        53

// Possible return codes from ProcessResponse
#define SUCCESS          1
#define TIMED_OUT        -1
#define INVALID_SERVER   -2
#define TRUNCATED        -3
#define INVALID_RESPONSE -4

void DNS_init(Dns *dns, const IP_address *aDNSServer);

int16_t DNS_inet_aton(Dns *dns, const char *aIPAddrString, IP_address *aResult);
int16_t DNS_getHostByName(Dns *dns, const char* aHostname, IP_address *aResult);

uint16_t DNS_buildRequest(Dns *dns, const char* aName);
uint16_t DNS_processResponse(Dns *dns, uint16_t aTimeout, IP_address *aAddress);


#endif  /* DNS_H */