/**
 * @file ethernet_server.h
 * @author Jose Roberto Parra Trewartha (uedsoldier1990@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-11-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef ETHERNET_SERVER_H
#define ETHERNET_SERVER_H

#include <stdint.h>
#include <stdbool.h>
#include "ethernet_client.h"

/**
 * @brief 
 * 
 */
typedef struct {
    uint16_t port;
    bool listening;
} Ethernet_server;

// Funciones
void EthernetServer_init(Ethernet_server *server, uint16_t port);
void EthernetServer_begin(Ethernet_server *server);
void EthernetServer_end(Ethernet_server *server);

size_t EthernetServer_writeToAllClients(Ethernet_server *server, const uint8_t *buf, size_t size);

#endif /* ETHERNET_SERVER_H */
