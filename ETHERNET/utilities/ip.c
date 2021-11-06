/**/

#include "ip.h"
#include "ipopt.h"
#include "ip_arch.h"

#if IP_CONF_IPV6
#include "ip-neighbor.h"
#endif /* IP_CONF_IPV6 */

#include <string.h>


/**
 * Definición de variables
*/
#if IP_FIXEDADDR > 0
const ipaddr_t ip_hostaddr = {HTONS((IP_IPADDR0 << 8) | IP_IPADDR1), HTONS((IP_IPADDR2 << 8) | IP_IPADDR3)};
const ipaddr_t ip_draddr =
  {HTONS((IP_DRIPADDR0 << 8) | IP_DRIPADDR1),
   HTONS((IP_DRIPADDR2 << 8) | IP_DRIPADDR3)};
const ipaddr_t ip_netmask =
  {HTONS((IP_NETMASK0 << 8) | IP_NETMASK1),
   HTONS((IP_NETMASK2 << 8) | IP_NETMASK3)};
#else
ipaddr_t ip_hostaddr, ip_draddr, ip_netmask;
#endif /* IP_FIXEDADDR */

static const ipaddr_t all_ones_addr =
#if IP_CONF_IPV6
  {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
#else /* IP_CONF_IPV6 */
  {0xffff,0xffff};
#endif /* IP_CONF_IPV6 */
static const ipaddr_t all_zeroes_addr =
#if IP_CONF_IPV6
  {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
#else /* IP_CONF_IPV6 */
  {0x0000,0x0000};
#endif /* IP_CONF_IPV6 */


#if IP_FIXEDETHADDR
const struct ip_eth_addr ip_ethaddr = {{IP_ETHADDR0,
					  IP_ETHADDR1,
					  IP_ETHADDR2,
					  IP_ETHADDR3,
					  IP_ETHADDR4,
					  IP_ETHADDR5}};
#else
struct ip_eth_addr ip_ethaddr = {{0,0,0,0,0,0}};
#endif

#ifndef IP_CONF_EXTERNAL_BUFFER
uint8_t ip_buf[IP_BUFSIZE + 2];   /* The packet buffer that contains
				    incoming packets. */
#endif /* IP_CONF_EXTERNAL_BUFFER */

void *ip_appdata;               /* The ip_appdata pointer points to
				    application data. */
void *ip_sappdata;              /* The ip_appdata pointer points to
				    the application data which is to
				    be sent. */
#if IP_URGDATA > 0
void *ip_urgdata;               /* The ip_urgdata pointer points to
   				    urgent data (out-of-band data), if
   				    present. */
uint16_t ip_urglen, ip_surglen;
#endif /* IP_URGDATA > 0 */

uint16_t ip_len, ip_slen;
                             /* The ip_len is either 8 or 16 bits,
				depending on the maximum packet
				size. */

uint8_t ip_flags;     /* The ip_flags variable is used for
				communication between the TCP/IP stack
				and the application program. */
struct ip_conn *ip_conn;   /* ip_conn always points to the current
				connection. */

struct ip_conn ip_conns[IP_CONNS];
                             /* The ip_conns array holds all TCP
				connections. */
uint16_t ip_listenports[IP_LISTENPORTS];
                             /* The ip_listenports list all currently
				listning ports. */
#if IP_UDP
struct ip_udp_conn *ip_udp_conn;
struct ip_udp_conn ip_udp_conns[IP_UDP_CONNS];
#endif /* IP_UDP */

static uint16_t ipid;           /* Ths ipid variable is an increasing
				number that is used for the IP ID
				field. */

void ip_setipid(uint16_t id) { ipid = id; }

static uint8_t iss[4];          /* The iss variable is used for the TCP
				initial sequence number. */

#if IP_ACTIVE_OPEN
static uint16_t lastport;       /* Keeps track of the last port used for
				a new connection. */
#endif /* IP_ACTIVE_OPEN */

/* Temporary variables. */
uint8_t ip_acc32[4];
static uint8_t c, opt;
static uint16_t tmp16;

/* Structures and definitions. */
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10
#define TCP_URG 0x20
#define TCP_CTL 0x3f

#define TCP_OPT_END     0   /* End of TCP options list */
#define TCP_OPT_NOOP    1   /* "No-operation" TCP option */
#define TCP_OPT_MSS     2   /* Maximum segment size TCP option */

#define TCP_OPT_MSS_LEN 4   /* Length of TCP MSS option. */

#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO       8

#define ICMP6_ECHO_REPLY             129
#define ICMP6_ECHO                   128
#define ICMP6_NEIGHBOR_SOLICITATION  135
#define ICMP6_NEIGHBOR_ADVERTISEMENT 136

#define ICMP6_FLAG_S (1 << 6)

#define ICMP6_OPTION_SOURCE_LINK_ADDRESS 1
#define ICMP6_OPTION_TARGET_LINK_ADDRESS 2


/* Macros. */
#define BUF ((struct ip_tcpip_hdr *)&ip_buf[IP_LLH_LEN])
#define FBUF ((struct ip_tcpip_hdr *)&ip_reassbuf[0])
#define ICMPBUF ((struct ip_icmpip_hdr *)&ip_buf[IP_LLH_LEN])
#define UDPBUF ((struct ip_udpip_hdr *)&ip_buf[IP_LLH_LEN])


#if IP_STATISTICS == 1
struct ip_stats ip_stat;
#define IP_STAT(s) s
#else
#define IP_STAT(s)
#endif /* IP_STATISTICS == 1 */

#if IP_LOGGING == 1
#include <stdio.h>
void ip_log(char *msg);
#define IP_LOG(m) ip_log(m)
#else
#define IP_LOG(m)
#endif /* IP_LOGGING == 1 */

#if ! IP_ARCH_ADD32
void
ip_add32(uint8_t *op32, uint16_t op16)
{
  ip_acc32[3] = op32[3] + (op16 & 0xff);
  ip_acc32[2] = op32[2] + (op16 >> 8);
  ip_acc32[1] = op32[1];
  ip_acc32[0] = op32[0];
  
  if(ip_acc32[2] < (op16 >> 8)) {
    ++ip_acc32[1];
    if(ip_acc32[1] == 0) {
      ++ip_acc32[0];
    }
  }
  
  
  if(ip_acc32[3] < (op16 & 0xff)) {
    ++ip_acc32[2];
    if(ip_acc32[2] == 0) {
      ++ip_acc32[1];
      if(ip_acc32[1] == 0) {
	++ip_acc32[0];
      }
    }
  }
}

#endif /* IP_ARCH_ADD32 */

#if ! IP_ARCH_CHKSUM
/*---------------------------------------------------------------------------*/
static uint16_t
chksum(uint16_t sum, const uint8_t *data, uint16_t len)
{
  uint16_t t;
  const uint8_t *dataptr;
  const uint8_t *last_byte;

  dataptr = data;
  last_byte = data + len - 1;
  
  while(dataptr < last_byte) {	/* At least two more bytes */
    t = (dataptr[0] << 8) + dataptr[1];
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
    dataptr += 2;
  }
  
  if(dataptr == last_byte) {
    t = (dataptr[0] << 8) + 0;
    sum += t;
    if(sum < t) {
      sum++;		/* carry */
    }
  }

  /* Return sum in host byte order. */
  return sum;
}
/*---------------------------------------------------------------------------*/
uint16_t
ip_chksum(uint16_t *data, uint16_t len)
{
  return htons(chksum(0, (uint8_t *)data, len));
}
/*---------------------------------------------------------------------------*/
#ifndef IP_ARCH_IPCHKSUM
uint16_t
ip_ipchksum(void)
{
  uint16_t sum;

  sum = chksum(0, &ip_buf[IP_LLH_LEN], IP_IPH_LEN);
  DEBUG_PRINTF("ip_ipchksum: sum 0x%04x\n", sum);
  return (sum == 0) ? 0xffff : htons(sum);
}
#endif
/*---------------------------------------------------------------------------*/
static uint16_t
upper_layer_chksum(uint8_t proto)
{
  uint16_t upper_layer_len;
  uint16_t sum;
  
#if IP_CONF_IPV6
  upper_layer_len = (((uint16_t)(BUF->len[0]) << 8) + BUF->len[1]);
#else /* IP_CONF_IPV6 */
  upper_layer_len = (((uint16_t)(BUF->len[0]) << 8) + BUF->len[1]) - IP_IPH_LEN;
#endif /* IP_CONF_IPV6 */
  
  /* First sum pseudoheader. */
  
  /* IP protocol and length fields. This addition cannot carry. */
  sum = upper_layer_len + proto;
  /* Sum IP source and destination addresses. */
  sum = chksum(sum, (uint8_t *)&BUF->srcipaddr[0], 2 * sizeof(ipaddr_t));

  /* Sum TCP header and data. */
  sum = chksum(sum, &ip_buf[IP_IPH_LEN + IP_LLH_LEN],
	       upper_layer_len);
    
  return (sum == 0) ? 0xffff : htons(sum);
}
/*---------------------------------------------------------------------------*/
#if IP_CONF_IPV6
uint16_t
ip_icmp6chksum(void)
{
  return upper_layer_chksum(IP_PROTO_ICMP6);
  
}
#endif /* IP_CONF_IPV6 */
/*---------------------------------------------------------------------------*/
uint16_t
ip_tcpchksum(void)
{
  return upper_layer_chksum(IP_PROTO_TCP);
}
/*---------------------------------------------------------------------------*/
#if IP_UDP_CHECKSUMS
uint16_t
ip_udpchksum(void)
{
  return upper_layer_chksum(IP_PROTO_UDP);
}
#endif /* IP_UDP_CHECKSUMS */
#endif /* IP_ARCH_CHKSUM */
/*---------------------------------------------------------------------------*/
void
ip_init(void)
{
  for(c = 0; c < IP_LISTENPORTS; ++c) {
    ip_listenports[c] = 0;
  }
  for(c = 0; c < IP_CONNS; ++c) {
    ip_conns[c].tcpstateflags = IP_CLOSED;
  }
#if IP_ACTIVE_OPEN
  lastport = 1024;
#endif /* IP_ACTIVE_OPEN */

#if IP_UDP
  for(c = 0; c < IP_UDP_CONNS; ++c) {
    ip_udp_conns[c].lport = 0;
  }
#endif /* IP_UDP */
  

  /* IPv4 initialization. */
#if IP_FIXEDADDR == 0
  /*  ip_hostaddr[0] = ip_hostaddr[1] = 0;*/
#endif /* IP_FIXEDADDR */

}
/*---------------------------------------------------------------------------*/
#if IP_ACTIVE_OPEN
struct ip_conn *
ip_connect(ipaddr_t *ripaddr, uint16_t rport)
{
  register struct ip_conn *conn, *cconn;
  
  /* Find an unused local port. */
 again:
  ++lastport;

  if(lastport >= 32000) {
    lastport = 4096;
  }

  /* Check if this port is already in use, and if so try to find
     another one. */
  for(c = 0; c < IP_CONNS; ++c) {
    conn = &ip_conns[c];
    if(conn->tcpstateflags != IP_CLOSED &&
       conn->lport == htons(lastport)) {
      goto again;
    }
  }

  conn = 0;
  for(c = 0; c < IP_CONNS; ++c) {
    cconn = &ip_conns[c];
    if(cconn->tcpstateflags == IP_CLOSED) {
      conn = cconn;
      break;
    }
    if(cconn->tcpstateflags == IP_TIME_WAIT) {
      if(conn == 0 ||
	 cconn->timer > conn->timer) {
	conn = cconn;
      }
    }
  }

  if(conn == 0) {
    return 0;
  }
  
  conn->tcpstateflags = IP_SYN_SENT;

  conn->snd_nxt[0] = iss[0];
  conn->snd_nxt[1] = iss[1];
  conn->snd_nxt[2] = iss[2];
  conn->snd_nxt[3] = iss[3];

  conn->initialmss = conn->mss = IP_TCP_MSS;
  
  conn->len = 1;   /* TCP length of the SYN is one. */
  conn->nrtx = 0;
  conn->timer = 1; /* Send the SYN next time around. */
  conn->rto = IP_RTO;
  conn->sa = 0;
  conn->sv = 16;   /* Initial value of the RTT variance. */
  conn->lport = htons(lastport);
  conn->rport = rport;
  ip_ipaddr_copy(&conn->ripaddr, ripaddr);
  
  return conn;
}
#endif /* IP_ACTIVE_OPEN */
/*---------------------------------------------------------------------------*/
#if IP_UDP
struct ip_udp_conn *
ip_udp_new(ipaddr_t *ripaddr, uint16_t rport)
{
  register struct ip_udp_conn *conn;
  
  /* Find an unused local port. */
 again:
  ++lastport;

  if(lastport >= 32000) {
    lastport = 4096;
  }
  
  for(c = 0; c < IP_UDP_CONNS; ++c) {
    if(ip_udp_conns[c].lport == htons(lastport)) {
      goto again;
    }
  }


  conn = 0;
  for(c = 0; c < IP_UDP_CONNS; ++c) {
    if(ip_udp_conns[c].lport == 0) {
      conn = &ip_udp_conns[c];
      break;
    }
  }

  if(conn == 0) {
    return 0;
  }
  
  conn->lport = HTONS(lastport);
  conn->rport = rport;
  if(ripaddr == NULL) {
    memset(&conn->ripaddr, 0, sizeof(ipaddr_t));
  } else {
    ip_ipaddr_copy(&conn->ripaddr, ripaddr);
  }
  conn->ttl = IP_TTL;
  
  return conn;
}
#endif /* IP_UDP */
/*---------------------------------------------------------------------------*/
void
ip_unlisten(uint16_t port)
{
  for(c = 0; c < IP_LISTENPORTS; ++c) {
    if(ip_listenports[c] == port) {
      ip_listenports[c] = 0;
      return;
    }
  }
}
/*---------------------------------------------------------------------------*/
void
ip_listen(uint16_t port)
{
  for(c = 0; c < IP_LISTENPORTS; ++c) {
    if(ip_listenports[c] == 0) {
      ip_listenports[c] = port;
      return;
    }
  }
}
/*---------------------------------------------------------------------------*/
/* XXX: IP fragment reassembly: not well-tested. */

#if IP_REASSEMBLY && !IP_CONF_IPV6
#define IP_REASS_BUFSIZE (IP_BUFSIZE - IP_LLH_LEN)
static uint8_t ip_reassbuf[IP_REASS_BUFSIZE];
static uint8_t ip_reassbitmap[IP_REASS_BUFSIZE / (8 * 8)];
static const uint8_t bitmap_bits[8] = {0xff, 0x7f, 0x3f, 0x1f,
				    0x0f, 0x07, 0x03, 0x01};
static uint16_t ip_reasslen;
static uint8_t ip_reassflags;
#define IP_REASS_FLAG_LASTFRAG 0x01
static uint8_t ip_reasstmr;

#define IP_MF   0x20

static uint8_t
ip_reass(void)
{
  uint16_t offset, len;
  uint16_t i;

  /* If ip_reasstmr is zero, no packet is present in the buffer, so we
     write the IP header of the fragment into the reassembly
     buffer. The timer is updated with the maximum age. */
  if(ip_reasstmr == 0) {
    memcpy(ip_reassbuf, &BUF->vhl, IP_IPH_LEN);
    ip_reasstmr = IP_REASS_MAXAGE;
    ip_reassflags = 0;
    /* Clear the bitmap. */
    memset(ip_reassbitmap, 0, sizeof(ip_reassbitmap));
  }

  /* Check if the incoming fragment matches the one currently present
     in the reasembly buffer. If so, we proceed with copying the
     fragment into the buffer. */
  if(BUF->srcipaddr[0] == FBUF->srcipaddr[0] &&
     BUF->srcipaddr[1] == FBUF->srcipaddr[1] &&
     BUF->destipaddr[0] == FBUF->destipaddr[0] &&
     BUF->destipaddr[1] == FBUF->destipaddr[1] &&
     BUF->ipid[0] == FBUF->ipid[0] &&
     BUF->ipid[1] == FBUF->ipid[1]) {

    len = (BUF->len[0] << 8) + BUF->len[1] - (BUF->vhl & 0x0f) * 4;
    offset = (((BUF->ipoffset[0] & 0x3f) << 8) + BUF->ipoffset[1]) * 8;

    /* If the offset or the offset + fragment length overflows the
       reassembly buffer, we discard the entire packet. */
    if(offset > IP_REASS_BUFSIZE ||
       offset + len > IP_REASS_BUFSIZE) {
      ip_reasstmr = 0;
      goto nullreturn;
    }

    /* Copy the fragment into the reassembly buffer, at the right
       offset. */
    memcpy(&ip_reassbuf[IP_IPH_LEN + offset],
	   (char *)BUF + (int)((BUF->vhl & 0x0f) * 4),
	   len);
      
    /* Update the bitmap. */
    if(offset / (8 * 8) == (offset + len) / (8 * 8)) {
      /* If the two endpoints are in the same byte, we only update
	 that byte. */
	     
      ip_reassbitmap[offset / (8 * 8)] |=
	     bitmap_bits[(offset / 8 ) & 7] &
	     ~bitmap_bits[((offset + len) / 8 ) & 7];
    } else {
      /* If the two endpoints are in different bytes, we update the
	 bytes in the endpoints and fill the stuff inbetween with
	 0xff. */
      ip_reassbitmap[offset / (8 * 8)] |=
	bitmap_bits[(offset / 8 ) & 7];
      for(i = 1 + offset / (8 * 8); i < (offset + len) / (8 * 8); ++i) {
	ip_reassbitmap[i] = 0xff;
      }
      ip_reassbitmap[(offset + len) / (8 * 8)] |=
	~bitmap_bits[((offset + len) / 8 ) & 7];
    }
    
    /* If this fragment has the More Fragments flag set to zero, we
       know that this is the last fragment, so we can calculate the
       size of the entire packet. We also set the
       IP_REASS_FLAG_LASTFRAG flag to indicate that we have received
       the final fragment. */

    if((BUF->ipoffset[0] & IP_MF) == 0) {
      ip_reassflags |= IP_REASS_FLAG_LASTFRAG;
      ip_reasslen = offset + len;
    }
    
    /* Finally, we check if we have a full packet in the buffer. We do
       this by checking if we have the last fragment and if all bits
       in the bitmap are set. */
    if(ip_reassflags & IP_REASS_FLAG_LASTFRAG) {
      /* Check all bytes up to and including all but the last byte in
	 the bitmap. */
      for(i = 0; i < ip_reasslen / (8 * 8) - 1; ++i) {
	if(ip_reassbitmap[i] != 0xff) {
	  goto nullreturn;
	}
      }
      /* Check the last byte in the bitmap. It should contain just the
	 right amount of bits. */
      if(ip_reassbitmap[ip_reasslen / (8 * 8)] !=
	 (uint8_t)~bitmap_bits[ip_reasslen / 8 & 7]) {
	goto nullreturn;
      }

      /* If we have come this far, we have a full packet in the
	 buffer, so we allocate a pbuf and copy the packet into it. We
	 also reset the timer. */
      ip_reasstmr = 0;
      memcpy(BUF, FBUF, ip_reasslen);

      /* Pretend to be a "normal" (i.e., not fragmented) IP packet
	 from now on. */
      BUF->ipoffset[0] = BUF->ipoffset[1] = 0;
      BUF->len[0] = ip_reasslen >> 8;
      BUF->len[1] = ip_reasslen & 0xff;
      BUF->ipchksum = 0;
      BUF->ipchksum = ~(ip_ipchksum());

      return ip_reasslen;
    }
  }

 nullreturn:
  return 0;
}
#endif /* IP_REASSEMBLY */
/*---------------------------------------------------------------------------*/
static void
ip_add_rcv_nxt(uint16_t n)
{
  ip_add32(ip_conn->rcv_nxt, n);
  ip_conn->rcv_nxt[0] = ip_acc32[0];
  ip_conn->rcv_nxt[1] = ip_acc32[1];
  ip_conn->rcv_nxt[2] = ip_acc32[2];
  ip_conn->rcv_nxt[3] = ip_acc32[3];
}
/*---------------------------------------------------------------------------*/
void
ip_process(uint8_t flag)
{
  register struct ip_conn *ip_connr = ip_conn;

#if IP_UDP
  if(flag == IP_UDP_SEND_CONN) {
    goto udp_send;
  }
#endif /* IP_UDP */
  
  ip_sappdata = ip_appdata = &ip_buf[IP_IPTCPH_LEN + IP_LLH_LEN];

  /* Check if we were invoked because of a poll request for a
     particular connection. */
  if(flag == IP_POLL_REQUEST) {
    if((ip_connr->tcpstateflags & IP_TS_MASK) == IP_ESTABLISHED &&
       !ip_outstanding(ip_connr)) {
	ip_flags = IP_POLL;
	IP_APPCALL();
	goto appsend;
    }
    goto drop;
    
    /* Check if we were invoked because of the perodic timer fireing. */
  } else if(flag == IP_TIMER) {
#if IP_REASSEMBLY
    if(ip_reasstmr != 0) {
      --ip_reasstmr;
    }
#endif /* IP_REASSEMBLY */
    /* Increase the initial sequence number. */
    if(++iss[3] == 0) {
      if(++iss[2] == 0) {
	if(++iss[1] == 0) {
	  ++iss[0];
	}
      }
    }

    /* Reset the length variables. */
    ip_len = 0;
    ip_slen = 0;

    /* Check if the connection is in a state in which we simply wait
       for the connection to time out. If so, we increase the
       connection's timer and remove the connection if it times
       out. */
    if(ip_connr->tcpstateflags == IP_TIME_WAIT ||
       ip_connr->tcpstateflags == IP_FIN_WAIT_2) {
      ++(ip_connr->timer);
      if(ip_connr->timer == IP_TIME_WAIT_TIMEOUT) {
	ip_connr->tcpstateflags = IP_CLOSED;
      }
    } else if(ip_connr->tcpstateflags != IP_CLOSED) {
      /* If the connection has outstanding data, we increase the
	 connection's timer and see if it has reached the RTO value
	 in which case we retransmit. */
      if(ip_outstanding(ip_connr)) {
	if(ip_connr->timer-- == 0) {
	  if(ip_connr->nrtx == IP_MAXRTX ||
	     ((ip_connr->tcpstateflags == IP_SYN_SENT ||
	       ip_connr->tcpstateflags == IP_SYN_RCVD) &&
	      ip_connr->nrtx == IP_MAXSYNRTX)) {
	    ip_connr->tcpstateflags = IP_CLOSED;

	    /* We call IP_APPCALL() with ip_flags set to
	       IP_TIMEDOUT to inform the application that the
	       connection has timed out. */
	    ip_flags = IP_TIMEDOUT;
	    IP_APPCALL();

	    /* We also send a reset packet to the remote host. */
	    BUF->flags = TCP_RST | TCP_ACK;
	    goto tcp_send_nodata;
	  }

	  /* Exponential backoff. */
	  ip_connr->timer = IP_RTO << (ip_connr->nrtx > 4?
					 4:
					 ip_connr->nrtx);
	  ++(ip_connr->nrtx);
	  
	  /* Ok, so we need to retransmit. We do this differently
	     depending on which state we are in. In ESTABLISHED, we
	     call upon the application so that it may prepare the
	     data for the retransmit. In SYN_RCVD, we resend the
	     SYNACK that we sent earlier and in LAST_ACK we have to
	     retransmit our FINACK. */
	  IP_STAT(++ip_stat.tcp.rexmit);
	  switch(ip_connr->tcpstateflags & IP_TS_MASK) {
	  case IP_SYN_RCVD:
	    /* In the SYN_RCVD state, we should retransmit our
               SYNACK. */
	    goto tcp_send_synack;
	    
#if IP_ACTIVE_OPEN
	  case IP_SYN_SENT:
	    /* In the SYN_SENT state, we retransmit out SYN. */
	    BUF->flags = 0;
	    goto tcp_send_syn;
#endif /* IP_ACTIVE_OPEN */
	    
	  case IP_ESTABLISHED:
	    /* In the ESTABLISHED state, we call upon the application
               to do the actual retransmit after which we jump into
               the code for sending out the packet (the apprexmit
               label). */
	    ip_flags = IP_REXMIT;
	    IP_APPCALL();
	    goto apprexmit;
	    
	  case IP_FIN_WAIT_1:
	  case IP_CLOSING:
	  case IP_LAST_ACK:
	    /* In all these states we should retransmit a FINACK. */
	    goto tcp_send_finack;
	    
	  }
	}
      } else if((ip_connr->tcpstateflags & IP_TS_MASK) == IP_ESTABLISHED) {
	/* If there was no need for a retransmission, we poll the
           application for new data. */
	ip_flags = IP_POLL;
	IP_APPCALL();
	goto appsend;
      }
    }
    goto drop;
  }
#if IP_UDP
  if(flag == IP_UDP_TIMER) {
    if(ip_udp_conn->lport != 0) {
      ip_conn = NULL;
      ip_sappdata = ip_appdata = &ip_buf[IP_LLH_LEN + IP_IPUDPH_LEN];
      ip_len = ip_slen = 0;
      ip_flags = IP_POLL;
      IP_UDP_APPCALL();
      goto udp_send;
    } else {
      goto drop;
    }
  }
#endif

  /* This is where the input processing starts. */
  IP_STAT(++ip_stat.ip.recv);

  /* Start of IP input header processing code. */
  
#if IP_CONF_IPV6
  /* Check validity of the IP header. */
  if((BUF->vtc & 0xf0) != 0x60)  { /* IP version and header length. */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.vhlerr);
    IP_LOG("ipv6: invalid version.");
    goto drop;
  }
#else /* IP_CONF_IPV6 */
  /* Check validity of the IP header. */
  if(BUF->vhl != 0x45)  { /* IP version and header length. */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.vhlerr);
    IP_LOG("ip: invalid version or header length.");
    goto drop;
  }
#endif /* IP_CONF_IPV6 */
  
  /* Check the size of the packet. If the size reported to us in
     ip_len is smaller the size reported in the IP header, we assume
     that the packet has been corrupted in transit. If the size of
     ip_len is larger than the size reported in the IP packet header,
     the packet has been padded and we set ip_len to the correct
     value.. */

  if((BUF->len[0] << 8) + BUF->len[1] <= ip_len) {
    ip_len = (BUF->len[0] << 8) + BUF->len[1];
#if IP_CONF_IPV6
    ip_len += 40; /* The length reported in the IPv6 header is the
		      length of the payload that follows the
		      header. However, uIP uses the ip_len variable
		      for holding the size of the entire packet,
		      including the IP header. For IPv4 this is not a
		      problem as the length field in the IPv4 header
		      contains the length of the entire packet. But
		      for IPv6 we need to add the size of the IPv6
		      header (40 bytes). */
#endif /* IP_CONF_IPV6 */
  } else {
    IP_LOG("ip: packet shorter than reported in IP header.");
    goto drop;
  }

#if !IP_CONF_IPV6
  /* Check the fragment flag. */
  if((BUF->ipoffset[0] & 0x3f) != 0 ||
     BUF->ipoffset[1] != 0) {
#if IP_REASSEMBLY
    ip_len = ip_reass();
    if(ip_len == 0) {
      goto drop;
    }
#else /* IP_REASSEMBLY */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.fragerr);
    IP_LOG("ip: fragment dropped.");
    goto drop;
#endif /* IP_REASSEMBLY */
  }
#endif /* IP_CONF_IPV6 */

  if(ip_ipaddr_cmp(ip_hostaddr, all_zeroes_addr)) {
    /* If we are configured to use ping IP address configuration and
       hasn't been assigned an IP address yet, we accept all ICMP
       packets. */
#if IP_PINGADDRCONF && !IP_CONF_IPV6
    if(BUF->proto == IP_PROTO_ICMP) {
      IP_LOG("ip: possible ping config packet received.");
      goto icmp_input;
    } else {
      IP_LOG("ip: packet dropped since no address assigned.");
      goto drop;
    }
#endif /* IP_PINGADDRCONF */

  } else {
    /* If IP broadcast support is configured, we check for a broadcast
       UDP packet, which may be destined to us. */
#if IP_BROADCAST
    DEBUG_PRINTF("UDP IP checksum 0x%04x\n", ip_ipchksum());
    if(BUF->proto == IP_PROTO_UDP &&
       ip_ipaddr_cmp(BUF->destipaddr, all_ones_addr)
       /*&&
	 ip_ipchksum() == 0xffff*/) {
      goto udp_input;
    }
#endif /* IP_BROADCAST */
    
    /* Check if the packet is destined for our IP address. */
#if !IP_CONF_IPV6
    if(!ip_ipaddr_cmp(BUF->destipaddr, ip_hostaddr)) {
      IP_STAT(++ip_stat.ip.drop);
      goto drop;
    }
#else /* IP_CONF_IPV6 */
    /* For IPv6, packet reception is a little trickier as we need to
       make sure that we listen to certain multicast addresses (all
       hosts multicast address, and the solicited-node multicast
       address) as well. However, we will cheat here and accept all
       multicast packets that are sent to the ff02::/16 addresses. */
    if(!ip_ipaddr_cmp(BUF->destipaddr, ip_hostaddr) &&
       BUF->destipaddr[0] != HTONS(0xff02)) {
      IP_STAT(++ip_stat.ip.drop);
      goto drop;
    }
#endif /* IP_CONF_IPV6 */
  }

#if !IP_CONF_IPV6
  if(ip_ipchksum() != 0xffff) { /* Compute and check the IP header
				    checksum. */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.chkerr);
    IP_LOG("ip: bad checksum.");
    goto drop;
  }
#endif /* IP_CONF_IPV6 */

  if(BUF->proto == IP_PROTO_TCP) { /* Check for TCP packet. If so,
				       proceed with TCP input
				       processing. */
    goto tcp_input;
  }

#if IP_UDP
  if(BUF->proto == IP_PROTO_UDP) {
    goto udp_input;
  }
#endif /* IP_UDP */

#if !IP_CONF_IPV6
  /* ICMPv4 processing code follows. */
  if(BUF->proto != IP_PROTO_ICMP) { /* We only allow ICMP packets from
					here. */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.protoerr);
    IP_LOG("ip: neither tcp nor icmp.");
    goto drop;
  }

#if IP_PINGADDRCONF
 icmp_input:
#endif /* IP_PINGADDRCONF */
  IP_STAT(++ip_stat.icmp.recv);

  /* ICMP echo (i.e., ping) processing. This is simple, we only change
     the ICMP type from ECHO to ECHO_REPLY and adjust the ICMP
     checksum before we return the packet. */
  if(ICMPBUF->type != ICMP_ECHO) {
    IP_STAT(++ip_stat.icmp.drop);
    IP_STAT(++ip_stat.icmp.typeerr);
    IP_LOG("icmp: not icmp echo.");
    goto drop;
  }

  /* If we are configured to use ping IP address assignment, we use
     the destination IP address of this ping packet and assign it to
     ourself. */
#if IP_PINGADDRCONF
  if((ip_hostaddr[0] | ip_hostaddr[1]) == 0) {
    ip_hostaddr[0] = BUF->destipaddr[0];
    ip_hostaddr[1] = BUF->destipaddr[1];
  }
#endif /* IP_PINGADDRCONF */

  ICMPBUF->type = ICMP_ECHO_REPLY;

  if(ICMPBUF->icmpchksum >= HTONS(0xffff - (ICMP_ECHO << 8))) {
    ICMPBUF->icmpchksum += HTONS(ICMP_ECHO << 8) + 1;
  } else {
    ICMPBUF->icmpchksum += HTONS(ICMP_ECHO << 8);
  }

  /* Swap IP addresses. */
  ip_ipaddr_copy(BUF->destipaddr, BUF->srcipaddr);
  ip_ipaddr_copy(BUF->srcipaddr, ip_hostaddr);

  IP_STAT(++ip_stat.icmp.sent);
  goto send;

  /* End of IPv4 input header processing code. */
#else /* !IP_CONF_IPV6 */

  /* This is IPv6 ICMPv6 processing code. */
  DEBUG_PRINTF("icmp6_input: length %d\n", ip_len);

  if(BUF->proto != IP_PROTO_ICMP6) { /* We only allow ICMPv6 packets from
					 here. */
    IP_STAT(++ip_stat.ip.drop);
    IP_STAT(++ip_stat.ip.protoerr);
    IP_LOG("ip: neither tcp nor icmp6.");
    goto drop;
  }

  IP_STAT(++ip_stat.icmp.recv);

  /* If we get a neighbor solicitation for our address we should send
     a neighbor advertisement message back. */
  if(ICMPBUF->type == ICMP6_NEIGHBOR_SOLICITATION) {
    if(ip_ipaddr_cmp(ICMPBUF->icmp6data, ip_hostaddr)) {

      if(ICMPBUF->options[0] == ICMP6_OPTION_SOURCE_LINK_ADDRESS) {
	/* Save the sender's address in our neighbor list. */
	ip_neighbor_add(ICMPBUF->srcipaddr, &(ICMPBUF->options[2]));
      }
      
      /* We should now send a neighbor advertisement back to where the
	 neighbor solicication came from. */
      ICMPBUF->type = ICMP6_NEIGHBOR_ADVERTISEMENT;
      ICMPBUF->flags = ICMP6_FLAG_S; /* Solicited flag. */
      
      ICMPBUF->reserved1 = ICMPBUF->reserved2 = ICMPBUF->reserved3 = 0;
      
      ip_ipaddr_copy(ICMPBUF->destipaddr, ICMPBUF->srcipaddr);
      ip_ipaddr_copy(ICMPBUF->srcipaddr, ip_hostaddr);
      ICMPBUF->options[0] = ICMP6_OPTION_TARGET_LINK_ADDRESS;
      ICMPBUF->options[1] = 1;  /* Options length, 1 = 8 bytes. */
      memcpy(&(ICMPBUF->options[2]), &ip_ethaddr, sizeof(ip_ethaddr));
      ICMPBUF->icmpchksum = 0;
      ICMPBUF->icmpchksum = ~ip_icmp6chksum();
      goto send;
      
    }
    goto drop;
  } else if(ICMPBUF->type == ICMP6_ECHO) {
    /* ICMP echo (i.e., ping) processing. This is simple, we only
       change the ICMP type from ECHO to ECHO_REPLY and update the
       ICMP checksum before we return the packet. */

    ICMPBUF->type = ICMP6_ECHO_REPLY;
    
    ip_ipaddr_copy(BUF->destipaddr, BUF->srcipaddr);
    ip_ipaddr_copy(BUF->srcipaddr, ip_hostaddr);
    ICMPBUF->icmpchksum = 0;
    ICMPBUF->icmpchksum = ~ip_icmp6chksum();
    
    IP_STAT(++ip_stat.icmp.sent);
    goto send;
  } else {
    DEBUG_PRINTF("Unknown icmp6 message type %d\n", ICMPBUF->type);
    IP_STAT(++ip_stat.icmp.drop);
    IP_STAT(++ip_stat.icmp.typeerr);
    IP_LOG("icmp: unknown ICMP message.");
    goto drop;
  }

  /* End of IPv6 ICMP processing. */
  
#endif /* !IP_CONF_IPV6 */

#if IP_UDP
  /* UDP input processing. */
 udp_input:
  /* UDP processing is really just a hack. We don't do anything to the
     UDP/IP headers, but let the UDP application do all the hard
     work. If the application sets ip_slen, it has a packet to
     send. */
#if IP_UDP_CHECKSUMS
  ip_len = ip_len - IP_IPUDPH_LEN;
  ip_appdata = &ip_buf[IP_LLH_LEN + IP_IPUDPH_LEN];
  if(UDPBUF->udpchksum != 0 && ip_udpchksum() != 0xffff) {
    IP_STAT(++ip_stat.udp.drop);
    IP_STAT(++ip_stat.udp.chkerr);
    IP_LOG("udp: bad checksum.");
    goto drop;
  }
#else /* IP_UDP_CHECKSUMS */
  ip_len = ip_len - IP_IPUDPH_LEN;
#endif /* IP_UDP_CHECKSUMS */

  /* Demultiplex this UDP packet between the UDP "connections". */
  for(ip_udp_conn = &ip_udp_conns[0];
      ip_udp_conn < &ip_udp_conns[IP_UDP_CONNS];
      ++ip_udp_conn) {
    /* If the local UDP port is non-zero, the connection is considered
       to be used. If so, the local port number is checked against the
       destination port number in the received packet. If the two port
       numbers match, the remote port number is checked if the
       connection is bound to a remote port. Finally, if the
       connection is bound to a remote IP address, the source IP
       address of the packet is checked. */
    if(ip_udp_conn->lport != 0 &&
       UDPBUF->destport == ip_udp_conn->lport &&
       (ip_udp_conn->rport == 0 ||
        UDPBUF->srcport == ip_udp_conn->rport) &&
       (ip_ipaddr_cmp(ip_udp_conn->ripaddr, all_zeroes_addr) ||
	ip_ipaddr_cmp(ip_udp_conn->ripaddr, all_ones_addr) ||
	ip_ipaddr_cmp(BUF->srcipaddr, ip_udp_conn->ripaddr))) {
      goto udp_found;
    }
  }
  IP_LOG("udp: no matching connection found");
  goto drop;
  
 udp_found:
  ip_conn = NULL;
  ip_flags = IP_NEWDATA;
  ip_sappdata = ip_appdata = &ip_buf[IP_LLH_LEN + IP_IPUDPH_LEN];
  ip_slen = 0;
  IP_UDP_APPCALL();
 udp_send:
  if(ip_slen == 0) {
    goto drop;
  }
  ip_len = ip_slen + IP_IPUDPH_LEN;

#if IP_CONF_IPV6
  /* For IPv6, the IP length field does not include the IPv6 IP header
     length. */
  BUF->len[0] = ((ip_len - IP_IPH_LEN) >> 8);
  BUF->len[1] = ((ip_len - IP_IPH_LEN) & 0xff);
#else /* IP_CONF_IPV6 */
  BUF->len[0] = (ip_len >> 8);
  BUF->len[1] = (ip_len & 0xff);
#endif /* IP_CONF_IPV6 */

  BUF->ttl = ip_udp_conn->ttl;
  BUF->proto = IP_PROTO_UDP;

  UDPBUF->udplen = HTONS(ip_slen + IP_UDPH_LEN);
  UDPBUF->udpchksum = 0;

  BUF->srcport  = ip_udp_conn->lport;
  BUF->destport = ip_udp_conn->rport;

  ip_ipaddr_copy(BUF->srcipaddr, ip_hostaddr);
  ip_ipaddr_copy(BUF->destipaddr, ip_udp_conn->ripaddr);
   
  ip_appdata = &ip_buf[IP_LLH_LEN + IP_IPTCPH_LEN];

#if IP_UDP_CHECKSUMS
  /* Calculate UDP checksum. */
  UDPBUF->udpchksum = ~(ip_udpchksum());
  if(UDPBUF->udpchksum == 0) {
    UDPBUF->udpchksum = 0xffff;
  }
#endif /* IP_UDP_CHECKSUMS */
  
  goto ip_send_nolen;
#endif /* IP_UDP */
  
  /* TCP input processing. */
 tcp_input:
  IP_STAT(++ip_stat.tcp.recv);

  /* Start of TCP input header processing code. */
  
  if(ip_tcpchksum() != 0xffff) {   /* Compute and check the TCP
				       checksum. */
    IP_STAT(++ip_stat.tcp.drop);
    IP_STAT(++ip_stat.tcp.chkerr);
    IP_LOG("tcp: bad checksum.");
    goto drop;
  }
  
  
  /* Demultiplex this segment. */
  /* First check any active connections. */
  for(ip_connr = &ip_conns[0]; ip_connr <= &ip_conns[IP_CONNS - 1];
      ++ip_connr) {
    if(ip_connr->tcpstateflags != IP_CLOSED &&
       BUF->destport == ip_connr->lport &&
       BUF->srcport == ip_connr->rport &&
       ip_ipaddr_cmp(BUF->srcipaddr, ip_connr->ripaddr)) {
      goto found;
    }
  }

  /* If we didn't find and active connection that expected the packet,
     either this packet is an old duplicate, or this is a SYN packet
     destined for a connection in LISTEN. If the SYN flag isn't set,
     it is an old packet and we send a RST. */
  if((BUF->flags & TCP_CTL) != TCP_SYN) {
    goto reset;
  }
  
  tmp16 = BUF->destport;
  /* Next, check listening connections. */
  for(c = 0; c < IP_LISTENPORTS; ++c) {
    if(tmp16 == ip_listenports[c])
      goto found_listen;
  }
  
  /* No matching connection found, so we send a RST packet. */
  IP_STAT(++ip_stat.tcp.synrst);
 reset:

  /* We do not send resets in response to resets. */
  if(BUF->flags & TCP_RST) {
    goto drop;
  }

  IP_STAT(++ip_stat.tcp.rst);
  
  BUF->flags = TCP_RST | TCP_ACK;
  ip_len = IP_IPTCPH_LEN;
  BUF->tcpoffset = 5 << 4;

  /* Flip the seqno and ackno fields in the TCP header. */
  c = BUF->seqno[3];
  BUF->seqno[3] = BUF->ackno[3];
  BUF->ackno[3] = c;
  
  c = BUF->seqno[2];
  BUF->seqno[2] = BUF->ackno[2];
  BUF->ackno[2] = c;
  
  c = BUF->seqno[1];
  BUF->seqno[1] = BUF->ackno[1];
  BUF->ackno[1] = c;
  
  c = BUF->seqno[0];
  BUF->seqno[0] = BUF->ackno[0];
  BUF->ackno[0] = c;

  /* We also have to increase the sequence number we are
     acknowledging. If the least significant byte overflowed, we need
     to propagate the carry to the other bytes as well. */
  if(++BUF->ackno[3] == 0) {
    if(++BUF->ackno[2] == 0) {
      if(++BUF->ackno[1] == 0) {
	++BUF->ackno[0];
      }
    }
  }
 
  /* Swap port numbers. */
  tmp16 = BUF->srcport;
  BUF->srcport = BUF->destport;
  BUF->destport = tmp16;
  
  /* Swap IP addresses. */
  ip_ipaddr_copy(BUF->destipaddr, BUF->srcipaddr);
  ip_ipaddr_copy(BUF->srcipaddr, ip_hostaddr);
  
  /* And send out the RST packet! */
  goto tcp_send_noconn;

  /* This label will be jumped to if we matched the incoming packet
     with a connection in LISTEN. In that case, we should create a new
     connection and send a SYNACK in return. */
 found_listen:
  /* First we check if there are any connections avaliable. Unused
     connections are kept in the same table as used connections, but
     unused ones have the tcpstate set to CLOSED. Also, connections in
     TIME_WAIT are kept track of and we'll use the oldest one if no
     CLOSED connections are found. Thanks to Eddie C. Dost for a very
     nice algorithm for the TIME_WAIT search. */
  ip_connr = 0;
  for(c = 0; c < IP_CONNS; ++c) {
    if(ip_conns[c].tcpstateflags == IP_CLOSED) {
      ip_connr = &ip_conns[c];
      break;
    }
    if(ip_conns[c].tcpstateflags == IP_TIME_WAIT) {
      if(ip_connr == 0 ||
	 ip_conns[c].timer > ip_connr->timer) {
	ip_connr = &ip_conns[c];
      }
    }
  }

  if(ip_connr == 0) {
    /* All connections are used already, we drop packet and hope that
       the remote end will retransmit the packet at a time when we
       have more spare connections. */
    IP_STAT(++ip_stat.tcp.syndrop);
    IP_LOG("tcp: found no unused connections.");
    goto drop;
  }
  ip_conn = ip_connr;
  
  /* Fill in the necessary fields for the new connection. */
  ip_connr->rto = ip_connr->timer = IP_RTO;
  ip_connr->sa = 0;
  ip_connr->sv = 4;
  ip_connr->nrtx = 0;
  ip_connr->lport = BUF->destport;
  ip_connr->rport = BUF->srcport;
  ip_ipaddr_copy(ip_connr->ripaddr, BUF->srcipaddr);
  ip_connr->tcpstateflags = IP_SYN_RCVD;

  ip_connr->snd_nxt[0] = iss[0];
  ip_connr->snd_nxt[1] = iss[1];
  ip_connr->snd_nxt[2] = iss[2];
  ip_connr->snd_nxt[3] = iss[3];
  ip_connr->len = 1;

  /* rcv_nxt should be the seqno from the incoming packet + 1. */
  ip_connr->rcv_nxt[3] = BUF->seqno[3];
  ip_connr->rcv_nxt[2] = BUF->seqno[2];
  ip_connr->rcv_nxt[1] = BUF->seqno[1];
  ip_connr->rcv_nxt[0] = BUF->seqno[0];
  ip_add_rcv_nxt(1);

  /* Parse the TCP MSS option, if present. */
  if((BUF->tcpoffset & 0xf0) > 0x50) {
    for(c = 0; c < ((BUF->tcpoffset >> 4) - 5) << 2 ;) {
      opt = ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + c];
      if(opt == TCP_OPT_END) {
	/* End of options. */
	break;
      } else if(opt == TCP_OPT_NOOP) {
	++c;
	/* NOP option. */
      } else if(opt == TCP_OPT_MSS &&
		ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c] == TCP_OPT_MSS_LEN) {
	/* An MSS option with the right option length. */
	tmp16 = ((uint16_t)ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 2 + c] << 8) |
	  (uint16_t)ip_buf[IP_IPTCPH_LEN + IP_LLH_LEN + 3 + c];
	ip_connr->initialmss = ip_connr->mss =
	  tmp16 > IP_TCP_MSS? IP_TCP_MSS: tmp16;
	
	/* And we are done processing options. */
	break;
      } else {
	/* All other options have a length field, so that we easily
	   can skip past them. */
	if(ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c] == 0) {
	  /* If the length field is zero, the options are malformed
	     and we don't process them further. */
	  break;
	}
	c += ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c];
      }
    }
  }
  
  /* Our response will be a SYNACK. */
#if IP_ACTIVE_OPEN
 tcp_send_synack:
  BUF->flags = TCP_ACK;
  
 tcp_send_syn:
  BUF->flags |= TCP_SYN;
#else /* IP_ACTIVE_OPEN */
 tcp_send_synack:
  BUF->flags = TCP_SYN | TCP_ACK;
#endif /* IP_ACTIVE_OPEN */
  
  /* We send out the TCP Maximum Segment Size option with our
     SYNACK. */
  BUF->optdata[0] = TCP_OPT_MSS;
  BUF->optdata[1] = TCP_OPT_MSS_LEN;
  BUF->optdata[2] = (IP_TCP_MSS) / 256;
  BUF->optdata[3] = (IP_TCP_MSS) & 255;
  ip_len = IP_IPTCPH_LEN + TCP_OPT_MSS_LEN;
  BUF->tcpoffset = ((IP_TCPH_LEN + TCP_OPT_MSS_LEN) / 4) << 4;
  goto tcp_send;

  /* This label will be jumped to if we found an active connection. */
 found:
  ip_conn = ip_connr;
  ip_flags = 0;
  /* We do a very naive form of TCP reset processing; we just accept
     any RST and kill our connection. We should in fact check if the
     sequence number of this reset is wihtin our advertised window
     before we accept the reset. */
  if(BUF->flags & TCP_RST) {
    ip_connr->tcpstateflags = IP_CLOSED;
    IP_LOG("tcp: got reset, aborting connection.");
    ip_flags = IP_ABORT;
    IP_APPCALL();
    goto drop;
  }
  /* Calculated the length of the data, if the application has sent
     any data to us. */
  c = (BUF->tcpoffset >> 4) << 2;
  /* ip_len will contain the length of the actual TCP data. This is
     calculated by subtracing the length of the TCP header (in
     c) and the length of the IP header (20 bytes). */
  ip_len = ip_len - c - IP_IPH_LEN;

  /* First, check if the sequence number of the incoming packet is
     what we're expecting next. If not, we send out an ACK with the
     correct numbers in. */
  if(!(((ip_connr->tcpstateflags & IP_TS_MASK) == IP_SYN_SENT) &&
       ((BUF->flags & TCP_CTL) == (TCP_SYN | TCP_ACK)))) {
    if((ip_len > 0 || ((BUF->flags & (TCP_SYN | TCP_FIN)) != 0)) &&
       (BUF->seqno[0] != ip_connr->rcv_nxt[0] ||
	BUF->seqno[1] != ip_connr->rcv_nxt[1] ||
	BUF->seqno[2] != ip_connr->rcv_nxt[2] ||
	BUF->seqno[3] != ip_connr->rcv_nxt[3])) {
      goto tcp_send_ack;
    }
  }

  /* Next, check if the incoming segment acknowledges any outstanding
     data. If so, we update the sequence number, reset the length of
     the outstanding data, calculate RTT estimations, and reset the
     retransmission timer. */
  if((BUF->flags & TCP_ACK) && ip_outstanding(ip_connr)) {
    ip_add32(ip_connr->snd_nxt, ip_connr->len);

    if(BUF->ackno[0] == ip_acc32[0] &&
       BUF->ackno[1] == ip_acc32[1] &&
       BUF->ackno[2] == ip_acc32[2] &&
       BUF->ackno[3] == ip_acc32[3]) {
      /* Update sequence number. */
      ip_connr->snd_nxt[0] = ip_acc32[0];
      ip_connr->snd_nxt[1] = ip_acc32[1];
      ip_connr->snd_nxt[2] = ip_acc32[2];
      ip_connr->snd_nxt[3] = ip_acc32[3];
	

      /* Do RTT estimation, unless we have done retransmissions. */
      if(ip_connr->nrtx == 0) {
	signed char m;
	m = ip_connr->rto - ip_connr->timer;
	/* This is taken directly from VJs original code in his paper */
	m = m - (ip_connr->sa >> 3);
	ip_connr->sa += m;
	if(m < 0) {
	  m = -m;
	}
	m = m - (ip_connr->sv >> 2);
	ip_connr->sv += m;
	ip_connr->rto = (ip_connr->sa >> 3) + ip_connr->sv;

      }
      /* Set the acknowledged flag. */
      ip_flags = IP_ACKDATA;
      /* Reset the retransmission timer. */
      ip_connr->timer = ip_connr->rto;

      /* Reset length of outstanding data. */
      ip_connr->len = 0;
    }
    
  }

  /* Do different things depending on in what state the connection is. */
  switch(ip_connr->tcpstateflags & IP_TS_MASK) {
    /* CLOSED and LISTEN are not handled here. CLOSE_WAIT is not
	implemented, since we force the application to close when the
	peer sends a FIN (hence the application goes directly from
	ESTABLISHED to LAST_ACK). */
  case IP_SYN_RCVD:
    /* In SYN_RCVD we have sent out a SYNACK in response to a SYN, and
       we are waiting for an ACK that acknowledges the data we sent
       out the last time. Therefore, we want to have the IP_ACKDATA
       flag set. If so, we enter the ESTABLISHED state. */
    if(ip_flags & IP_ACKDATA) {
      ip_connr->tcpstateflags = IP_ESTABLISHED;
      ip_flags = IP_CONNECTED;
      ip_connr->len = 0;
      if(ip_len > 0) {
        ip_flags |= IP_NEWDATA;
        ip_add_rcv_nxt(ip_len);
      }
      ip_slen = 0;
      IP_APPCALL();
      goto appsend;
    }
    goto drop;
#if IP_ACTIVE_OPEN
  case IP_SYN_SENT:
    /* In SYN_SENT, we wait for a SYNACK that is sent in response to
       our SYN. The rcv_nxt is set to sequence number in the SYNACK
       plus one, and we send an ACK. We move into the ESTABLISHED
       state. */
    if((ip_flags & IP_ACKDATA) &&
       (BUF->flags & TCP_CTL) == (TCP_SYN | TCP_ACK)) {

      /* Parse the TCP MSS option, if present. */
      if((BUF->tcpoffset & 0xf0) > 0x50) {
	for(c = 0; c < ((BUF->tcpoffset >> 4) - 5) << 2 ;) {
	  opt = ip_buf[IP_IPTCPH_LEN + IP_LLH_LEN + c];
	  if(opt == TCP_OPT_END) {
	    /* End of options. */
	    break;
	  } else if(opt == TCP_OPT_NOOP) {
	    ++c;
	    /* NOP option. */
	  } else if(opt == TCP_OPT_MSS &&
		    ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c] == TCP_OPT_MSS_LEN) {
	    /* An MSS option with the right option length. */
	    tmp16 = (ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 2 + c] << 8) |
	      ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 3 + c];
	    ip_connr->initialmss =
	      ip_connr->mss = tmp16 > IP_TCP_MSS? IP_TCP_MSS: tmp16;

	    /* And we are done processing options. */
	    break;
	  } else {
	    /* All other options have a length field, so that we easily
	       can skip past them. */
	    if(ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c] == 0) {
	      /* If the length field is zero, the options are malformed
		 and we don't process them further. */
	      break;
	    }
	    c += ip_buf[IP_TCPIP_HLEN + IP_LLH_LEN + 1 + c];
	  }
	}
      }
      ip_connr->tcpstateflags = IP_ESTABLISHED;
      ip_connr->rcv_nxt[0] = BUF->seqno[0];
      ip_connr->rcv_nxt[1] = BUF->seqno[1];
      ip_connr->rcv_nxt[2] = BUF->seqno[2];
      ip_connr->rcv_nxt[3] = BUF->seqno[3];
      ip_add_rcv_nxt(1);
      ip_flags = IP_CONNECTED | IP_NEWDATA;
      ip_connr->len = 0;
      ip_len = 0;
      ip_slen = 0;
      IP_APPCALL();
      goto appsend;
    }
    /* Inform the application that the connection failed */
    ip_flags = IP_ABORT;
    IP_APPCALL();
    /* The connection is closed after we send the RST */
    ip_conn->tcpstateflags = IP_CLOSED;
    goto reset;
#endif /* IP_ACTIVE_OPEN */
    
  case IP_ESTABLISHED:
    /* In the ESTABLISHED state, we call upon the application to feed
    data into the ip_buf. If the IP_ACKDATA flag is set, the
    application should put new data into the buffer, otherwise we are
    retransmitting an old segment, and the application should put that
    data into the buffer.
    If the incoming packet is a FIN, we should close the connection on
    this side as well, and we send out a FIN and enter the LAST_ACK
    state. We require that there is no outstanding data; otherwise the
    sequence numbers will be screwed up. */

    if(BUF->flags & TCP_FIN && !(ip_connr->tcpstateflags & IP_STOPPED)) {
      if(ip_outstanding(ip_connr)) {
	goto drop;
      }
      ip_add_rcv_nxt(1 + ip_len);
      ip_flags |= IP_CLOSE;
      if(ip_len > 0) {
	ip_flags |= IP_NEWDATA;
      }
      IP_APPCALL();
      ip_connr->len = 1;
      ip_connr->tcpstateflags = IP_LAST_ACK;
      ip_connr->nrtx = 0;
    tcp_send_finack:
      BUF->flags = TCP_FIN | TCP_ACK;
      goto tcp_send_nodata;
    }

    /* Check the URG flag. If this is set, the segment carries urgent
       data that we must pass to the application. */
    if((BUF->flags & TCP_URG) != 0) {
#if IP_URGDATA > 0
      ip_urglen = (BUF->urgp[0] << 8) | BUF->urgp[1];
      if(ip_urglen > ip_len) {
	/* There is more urgent data in the next segment to come. */
	ip_urglen = ip_len;
      }
      ip_add_rcv_nxt(ip_urglen);
      ip_len -= ip_urglen;
      ip_urgdata = ip_appdata;
      ip_appdata += ip_urglen;
    } else {
      ip_urglen = 0;
#else /* IP_URGDATA > 0 */
      ip_appdata = ((char *)ip_appdata) + ((BUF->urgp[0] << 8) | BUF->urgp[1]);
      ip_len -= (BUF->urgp[0] << 8) | BUF->urgp[1];
#endif /* IP_URGDATA > 0 */
    }

    /* If ip_len > 0 we have TCP data in the packet, and we flag this
       by setting the IP_NEWDATA flag and update the sequence number
       we acknowledge. If the application has stopped the dataflow
       using ip_stop(), we must not accept any data packets from the
       remote host. */
    if(ip_len > 0 && !(ip_connr->tcpstateflags & IP_STOPPED)) {
      ip_flags |= IP_NEWDATA;
      ip_add_rcv_nxt(ip_len);
    }

    /* Check if the available buffer space advertised by the other end
       is smaller than the initial MSS for this connection. If so, we
       set the current MSS to the window size to ensure that the
       application does not send more data than the other end can
       handle.
       If the remote host advertises a zero window, we set the MSS to
       the initial MSS so that the application will send an entire MSS
       of data. This data will not be acknowledged by the receiver,
       and the application will retransmit it. This is called the
       "persistent timer" and uses the retransmission mechanim.
    */
    tmp16 = ((uint16_t)BUF->wnd[0] << 8) + (uint16_t)BUF->wnd[1];
    if(tmp16 > ip_connr->initialmss ||
       tmp16 == 0) {
      tmp16 = ip_connr->initialmss;
    }
    ip_connr->mss = tmp16;

    /* If this packet constitutes an ACK for outstanding data (flagged
       by the IP_ACKDATA flag, we should call the application since it
       might want to send more data. If the incoming packet had data
       from the peer (as flagged by the IP_NEWDATA flag), the
       application must also be notified.
       When the application is called, the global variable ip_len
       contains the length of the incoming data. The application can
       access the incoming data through the global pointer
       ip_appdata, which usually points IP_IPTCPH_LEN + IP_LLH_LEN
       bytes into the ip_buf array.
       If the application wishes to send any data, this data should be
       put into the ip_appdata and the length of the data should be
       put into ip_len. If the application don't have any data to
       send, ip_len must be set to 0. */
    if(ip_flags & (IP_NEWDATA | IP_ACKDATA)) {
      ip_slen = 0;
      IP_APPCALL();

    appsend:
      
      if(ip_flags & IP_ABORT) {
	ip_slen = 0;
	ip_connr->tcpstateflags = IP_CLOSED;
	BUF->flags = TCP_RST | TCP_ACK;
	goto tcp_send_nodata;
      }

      if(ip_flags & IP_CLOSE) {
	ip_slen = 0;
	ip_connr->len = 1;
	ip_connr->tcpstateflags = IP_FIN_WAIT_1;
	ip_connr->nrtx = 0;
	BUF->flags = TCP_FIN | TCP_ACK;
	goto tcp_send_nodata;
      }

      /* If ip_slen > 0, the application has data to be sent. */
      if(ip_slen > 0) {

	/* If the connection has acknowledged data, the contents of
	   the ->len variable should be discarded. */
	if((ip_flags & IP_ACKDATA) != 0) {
	  ip_connr->len = 0;
	}

	/* If the ->len variable is non-zero the connection has
	   already data in transit and cannot send anymore right
	   now. */
	if(ip_connr->len == 0) {

	  /* The application cannot send more than what is allowed by
	     the mss (the minumum of the MSS and the available
	     window). */
	  if(ip_slen > ip_connr->mss) {
	    ip_slen = ip_connr->mss;
	  }

	  /* Remember how much data we send out now so that we know
	     when everything has been acknowledged. */
	  ip_connr->len = ip_slen;
	} else {

	  /* If the application already had unacknowledged data, we
	     make sure that the application does not send (i.e.,
	     retransmit) out more than it previously sent out. */
	  ip_slen = ip_connr->len;
	}
      }
      ip_connr->nrtx = 0;
    apprexmit:
      ip_appdata = ip_sappdata;
      
      /* If the application has data to be sent, or if the incoming
         packet had new data in it, we must send out a packet. */
      if(ip_slen > 0 && ip_connr->len > 0) {
	/* Add the length of the IP and TCP headers. */
	ip_len = ip_connr->len + IP_TCPIP_HLEN;
	/* We always set the ACK flag in response packets. */
	BUF->flags = TCP_ACK | TCP_PSH;
	/* Send the packet. */
	goto tcp_send_noopts;
      }
      /* If there is no data to send, just send out a pure ACK if
	 there is newdata. */
      if(ip_flags & IP_NEWDATA) {
	ip_len = IP_TCPIP_HLEN;
	BUF->flags = TCP_ACK;
	goto tcp_send_noopts;
      }
    }
    goto drop;
  case IP_LAST_ACK:
    /* We can close this connection if the peer has acknowledged our
       FIN. This is indicated by the IP_ACKDATA flag. */
    if(ip_flags & IP_ACKDATA) {
      ip_connr->tcpstateflags = IP_CLOSED;
      ip_flags = IP_CLOSE;
      IP_APPCALL();
    }
    break;
    
  case IP_FIN_WAIT_1:
    /* The application has closed the connection, but the remote host
       hasn't closed its end yet. Thus we do nothing but wait for a
       FIN from the other side. */
    if(ip_len > 0) {
      ip_add_rcv_nxt(ip_len);
    }
    if(BUF->flags & TCP_FIN) {
      if(ip_flags & IP_ACKDATA) {
	ip_connr->tcpstateflags = IP_TIME_WAIT;
	ip_connr->timer = 0;
	ip_connr->len = 0;
      } else {
	ip_connr->tcpstateflags = IP_CLOSING;
      }
      ip_add_rcv_nxt(1);
      ip_flags = IP_CLOSE;
      IP_APPCALL();
      goto tcp_send_ack;
    } else if(ip_flags & IP_ACKDATA) {
      ip_connr->tcpstateflags = IP_FIN_WAIT_2;
      ip_connr->len = 0;
      goto drop;
    }
    if(ip_len > 0) {
      goto tcp_send_ack;
    }
    goto drop;
      
  case IP_FIN_WAIT_2:
    if(ip_len > 0) {
      ip_add_rcv_nxt(ip_len);
    }
    if(BUF->flags & TCP_FIN) {
      ip_connr->tcpstateflags = IP_TIME_WAIT;
      ip_connr->timer = 0;
      ip_add_rcv_nxt(1);
      ip_flags = IP_CLOSE;
      IP_APPCALL();
      goto tcp_send_ack;
    }
    if(ip_len > 0) {
      goto tcp_send_ack;
    }
    goto drop;

  case IP_TIME_WAIT:
    goto tcp_send_ack;
    
  case IP_CLOSING:
    if(ip_flags & IP_ACKDATA) {
      ip_connr->tcpstateflags = IP_TIME_WAIT;
      ip_connr->timer = 0;
    }
  }
  goto drop;
  

  /* We jump here when we are ready to send the packet, and just want
     to set the appropriate TCP sequence numbers in the TCP header. */
 tcp_send_ack:
  BUF->flags = TCP_ACK;
 tcp_send_nodata:
  ip_len = IP_IPTCPH_LEN;
 tcp_send_noopts:
  BUF->tcpoffset = (IP_TCPH_LEN / 4) << 4;
 tcp_send:
  /* We're done with the input processing. We are now ready to send a
     reply. Our job is to fill in all the fields of the TCP and IP
     headers before calculating the checksum and finally send the
     packet. */
  BUF->ackno[0] = ip_connr->rcv_nxt[0];
  BUF->ackno[1] = ip_connr->rcv_nxt[1];
  BUF->ackno[2] = ip_connr->rcv_nxt[2];
  BUF->ackno[3] = ip_connr->rcv_nxt[3];
  
  BUF->seqno[0] = ip_connr->snd_nxt[0];
  BUF->seqno[1] = ip_connr->snd_nxt[1];
  BUF->seqno[2] = ip_connr->snd_nxt[2];
  BUF->seqno[3] = ip_connr->snd_nxt[3];

  BUF->proto = IP_PROTO_TCP;
  
  BUF->srcport  = ip_connr->lport;
  BUF->destport = ip_connr->rport;

  ip_ipaddr_copy(BUF->srcipaddr, ip_hostaddr);
  ip_ipaddr_copy(BUF->destipaddr, ip_connr->ripaddr);

  if(ip_connr->tcpstateflags & IP_STOPPED) {
    /* If the connection has issued ip_stop(), we advertise a zero
       window so that the remote host will stop sending data. */
    BUF->wnd[0] = BUF->wnd[1] = 0;
  } else {
    BUF->wnd[0] = ((IP_RECEIVE_WINDOW) >> 8);
    BUF->wnd[1] = ((IP_RECEIVE_WINDOW) & 0xff);
  }

 tcp_send_noconn:
  BUF->ttl = IP_TTL;
#if IP_CONF_IPV6
  /* For IPv6, the IP length field does not include the IPv6 IP header
     length. */
  BUF->len[0] = ((ip_len - IP_IPH_LEN) >> 8);
  BUF->len[1] = ((ip_len - IP_IPH_LEN) & 0xff);
#else /* IP_CONF_IPV6 */
  BUF->len[0] = (ip_len >> 8);
  BUF->len[1] = (ip_len & 0xff);
#endif /* IP_CONF_IPV6 */

  BUF->urgp[0] = BUF->urgp[1] = 0;
  
  /* Calculate TCP checksum. */
  BUF->tcpchksum = 0;
  BUF->tcpchksum = ~(ip_tcpchksum());
  
 ip_send_nolen:

#if IP_CONF_IPV6
  BUF->vtc = 0x60;
  BUF->tcflow = 0x00;
  BUF->flow = 0x00;
#else /* IP_CONF_IPV6 */
  BUF->vhl = 0x45;
  BUF->tos = 0;
  BUF->ipoffset[0] = BUF->ipoffset[1] = 0;
  ++ipid;
  BUF->ipid[0] = ipid >> 8;
  BUF->ipid[1] = ipid & 0xff;
  /* Calculate IP checksum. */
  BUF->ipchksum = 0;
  BUF->ipchksum = ~(ip_ipchksum());
  DEBUG_PRINTF("uip ip_send_nolen: chkecum 0x%04x\n", ip_ipchksum());
#endif /* IP_CONF_IPV6 */
   
  IP_STAT(++ip_stat.tcp.sent);
 send:
  DEBUG_PRINTF("Sending packet with length %d (%d)\n", ip_len,
	       (BUF->len[0] << 8) | BUF->len[1]);
  
  IP_STAT(++ip_stat.ip.sent);
  /* Return and let the caller do the actual transmission. */
  ip_flags = 0;
  return;
 drop:
  ip_len = 0;
  ip_flags = 0;
  return;
}
/*---------------------------------------------------------------------------*/
uint16_t
htons(uint16_t val)
{
  return HTONS(val);
}
/*---------------------------------------------------------------------------*/
void
ip_send(const void *data, int len)
{
  ip_slen = len;
  if(len > 0) {
    if(data != ip_sappdata) {
      memcpy(ip_sappdata, (data), ip_slen);
    }
  }
}