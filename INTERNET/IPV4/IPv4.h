#ifndef IPV4_H
#define IPV4_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../utils.h"

/**
 * @brief 
 */
#ifndef IPV4_LOG
#define IPV4_LOG 1
#endif

/**
 * @brief 
 * 
 */
#define IPV4_MAX_SIZE    16

#define IPV4_BYTE_COUNT     4
#define IPV4_STRING_SEPARATOR   '.'

#define IPV4_ADDRESS_NONE   0x00000000

/**
* @brief Definición de estructura de datos para direccionamiento IPv4
*/
typedef union IPV4_address {
    uint8_t ipv4_addr_array[IPV4_BYTE_COUNT];
    struct {
        uint8_t b0, b1, b2, b3;
    } ipv4_bytes;
    uint32_t ipv4_word;
} IPV4_address_t;

/**
 * @brief 
 * 
 */
typedef enum IPV4_error {
    IPV4_NULL_STRING, IPV4_NULL_TOKEN, IPV4_NaN, IPV4_INVALID_NUMBER, IPV4_INVALID_ADDRESS, IPV4_INVALID_ARRAY_LENGTH ,IPV4_ADDRESS_OK
} IPV4_error_t;

IPV4_error_t IPV4_fromArray(IPV4_address_t *address, uint8_t *bytes);
void array_fromIPV4(IPV4_address_t *address, uint8_t *bytes);
IPV4_error_t IPV4_fromString(IPV4_address_t *address, const char *string);
char *string_fromIPV4(IPV4_address_t *address);
bool IPV4_validMask(IPV4_address_t *subnetmask);
uint32_t IPV4_int_fromIPv4(IPV4_address_t *address);
char *IPV4_string_fromInt(uint32_t ip_int);
void IPV4_copy(IPV4_address_t *dest, IPV4_address_t *src );
bool IPV4_compare(IPV4_address_t *a1, IPV4_address_t *a2 );

#endif /* IPV4_H */