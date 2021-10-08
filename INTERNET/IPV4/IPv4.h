#ifndef IPV4_H
#define IPV4_H
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
//
#define IPV4_LOG 1

/**
* @brief Definición de estructura de datos para direccionamiento IPv4
*/
typedef union IPV4_address {
    uint8_t ipv4_addr_array[4];
    struct {
        uint8_t b0, b1, b2, b3;
    } ipv4_bytes;
    uint32_t ipv4_word;
} IPV4_address_t;

void makeIPfromArray(IPV4_address_t *address, uint8_t *bytes);
void makeArrayFromIP(IPV4_address_t *address, uint8_t *bytes);
void makeIPfromString(IPV4_address_t *address, char *string);
char *makeStringFromIP(IPV4_address_t *address);
bool IPV4_validMask(IPV4_address_t *subnetmask);
uint32_t makeIPintFromIP(IPV4_address_t *address);
char *makeStringFromInt(uint32_t ip_int);

#endif /* IPV4_H */