/**
 * @file ethernet_server.c
 * @author Jose Roberto Parra Trewartha (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "ethernet_server.h"

/**
 * @brief 
 * 
 * @param server 
 * @param port 
 */
void EthernetServer_init(Ethernet_server *server, uint16_t port) {
    server->port = port;
}

/**
 * @brief 
 * 
 * @param server 
 */
void EthernetServer_begin(Ethernet_server *server) {

}

/**
 * @brief 
 * 
 * @param server 
 */
void EthernetServer_end(Ethernet_server *server) {

}

/**
 * @brief 
 * 
 * @param server 
 * @param buf 
 * @param size 
 * @return size_t 
 */
size_t EthernetServer_writeToAllClients(Ethernet_server *server, const uint8_t *buf, size_t size) {

}
