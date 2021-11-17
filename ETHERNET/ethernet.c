/**
 * @file ethernet.c
 * @author José Roberto Parra Trewartha (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ethernet.h"

#define ETH_HDR ((struct ip_eth_hdr *)&ip_buf[0])

/**
 * @brief 
 * 
 * @param eth 
 * @param mac 
 */
void ip_ethernet_init(Ethernet *eth, const uint8_t *mac) {
    eth->initialized = ENC28J60_init(mac);
    eth->in_packet = NOBLOCK;
    eth->ip_packet = NOBLOCK;
    eth->ip_hdrlen = 0;
    eth->packetstate = 0;
    eth->_dnsServerAddress.ipv4_word = 0;
    eth->_dhcp = Dhcp
}

/**
 * @brief 
 * 
 * @param eth 
 * @param ip 
 * @param dns 
 * @param gateway 
 * @param subnet 
 */
void ip_ethernet_configure(Ethernet *eth, IP_address ip, IP_address dns, IP_address gateway, IP_address subnet) {

}

/**
 * @brief 
 * 
 * @param eth 
 */
void Ethernetick(Ethernet *eth) {

}

/**
 * @brief 
 * 
 * @param eth 
 * @return true 
 * @return false 
 */
bool ip_ethernet_network_send(Ethernet *eth) {

}

/**
 * @brief 
 * 
 * @param eth 
 */
void ip_ethernet_call_yield(Ethernet *eth) {

}

#if IP_UDP

/**
 * @brief 
 * 
 * @param eth 
 * @param proto 
 * @return uint16_t 
 */
uint16_t ip_ethernet_upper_layer_chksum(Ethernet *eth, uint8_t proto) {

}
#endif
   
#if IP_CONF_IPV6
/**
 * @brief 
 * 
 * @param eth 
 * @return uint16_t 
 */
uint16_t ip_icmp6chksum(Ethernet *eth) {

}
#endif /* IP_CONF_IPV6 */
/**
 * @brief 
 * 
 * @param eth 
 * @param sum 
 * @param data 
 * @param len 
 * @return uint16_t 
 */
uint16_t ip_ethernet_chksum(Ethernet *eth, uint16_t sum, const uint8_t* data, uint16_t len) {

}

/**
 * @brief 
 * 
 * @param eth 
 * @return uint16_t 
 */
uint16_t ip_ethernet_ipchksum(Ethernet *eth) {

}


