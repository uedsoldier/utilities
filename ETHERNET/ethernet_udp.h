#ifndef IPUDP_H
#define IPUDP_H

//#include "udp.h"
#include "utilities/mempool.h"
#include "utilities/util.h"
#include <stdint.h>
#include <stdbool.h>

#define IP_UDP_MAXDATALEN 1500
#define IP_UDP_PHYH_LEN IP_LLH_LEN + IP_IPUDPH_LEN
#define IP_UDP_MAXPACKETSIZE IP_UDP_MAXDATALEN + IP_UDP_PHYH_LEN

typedef struct
{
	memhandle packet;
	IP_address remote_ip;
	uint16_t remote_port;
} uip_udp_msg_rec_t;

typedef struct
{
	memaddress out_pos;
	uip_udp_msg_rec_t packet_next[IP_UDP_BACKLOG];
	memhandle packet_in;
	memhandle packet_out;
	bool send;
	IP_address remote_ip;
	uint16_t remote_port;
} uip_udp_userdata_t;

class EthernetUDP : public UDP
{

private:
	struct uip_udp_conn *_uip_udp_conn;

	uip_udp_userdata_t appdata;

public:
	EthernetUDP(); // Constructor
	uint8_t
		begin(uint16_t); // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
	void
	stop(); // Finish with the UDP socket

	// Sending UDP packets

	// Start building up a packet to send to the remote host specific in ip and port
	// Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
	int
	beginPacket(IP_address ip, uint16_t port);
	// Start building up a packet to send to the remote host specific in host and port
	// Returns 1 if successful, 0 if there was a problem resolving the hostname or port
	int
	beginPacket(const char *host, uint16_t port);
	// Finish off this packet and send it
	// Returns 1 if the packet was sent successfully, 0 if there was an error
	int
	endPacket();
	// Write a single byte into the packet
	size_t
		write(uint8_t);
	// Write size bytes from buffer into the packet
	size_t
	write(const uint8_t *buffer, size_t size);
	// flush() should send the data, but here endPacket does it
	void
	flush(){}; // in Print it is empty, UDP makes it pure virtual

	using Print::write;

	// Start processing the next available incoming packet
	// Returns the size of the packet in bytes, or 0 if no packets are available
	int
	parsePacket();
	// Number of bytes remaining in the current packet
	int
	available();
	// Read a single byte from the current packet
	int
	read();
	// Read up to len bytes from the current packet and place them into buffer
	// Returns the number of bytes read, or 0 if none are available
	int
	read(unsigned char *buffer, size_t len);
	// Read up to len characters from the current packet and place them into buffer
	// Returns the number of characters read, or 0 if none are available
	int
	read(char *buffer, size_t len)
	{
		return read((unsigned char *)buffer, len);
	};
	// Return the next byte from the current packet without moving on to the next byte
	int
	peek();
	void
	discardReceived(); // former flush

	// Return the IP address of the host who sent the current incoming packet
	IP_address
	remoteIP();

	// Return the port of the host who sent the current incoming packet
	uint16_t
	remotePort();

private:
	friend void uipudp_appcall(void);

	friend class UIPEthernetClass;
	static void _send(struct uip_udp_conn *uip_udp_conn);

	static uint8_t _newBlock(uip_udp_msg_rec_t *blocks);
	static void _moveBlocks(uip_udp_msg_rec_t *blocks);
	static void _flushBlocks(uip_udp_msg_rec_t *blocks);
};

#endif /* IPUDP_H */