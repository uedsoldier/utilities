#ifndef IPV6_H
#define IPV6_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../../utils.h"

/**
 * @brief Macro para activar o desactivar logging por puerto serie
 */
#ifndef IPV6_LOG
#define	IPV6_LOG	1
#endif

#define IPV6_LOWERCASE false
#define IPV6_UPPERCASE true

/**
 * @brief 
 * 
 */
#define IPV6_MAX_SIZE    45


#define IPV6_WORD_COUNT     8
#define IPV6_STRING_SEPARATOR   ':'

/**
* @brief Definición de estructura de datos para direccionamiento IPv6
*/
typedef union IPV6_address {
    uint16_t ipv6_addr_array[8];
    struct {
        uint16_t w0, w1, w2, w3, w4, w5, w6, w7;
    } ipv6_words;
    uint32_t ipv6_word;
} IPV6_address_t;

/**
 * @brief 
 * 
 */
typedef enum IPV4_error {
    IPV6_NULL_STRING, IPV6_NULL_TOKEN, IPV6_NaN, IPV6_INVALID_NUMBER, IPV6_INVALID_ADDRESS, IPV6_INVALID_ARRAY_LENGTH ,IPV6_ADDRESS_OK
} IPV6_error_t;

IPV6_error_t IPV6_fromArray(IPV6_address_t *address, uint16_t *words);
void array_fromIPV6(IPV6_address_t *address, uint16_t *words);
IPV6_error_t IPV6_fromString(IPV6_address_t *address, const char *string);
char *string_fromIPV6(IPV6_address_t *address, bool upper);
void IPV6_copy(IPV6_address_t *dest, IPV6_address_t *src);
bool IPV6_compare(IPV6_address_t *a1, IPV6_address_t *a2 );

#endif /* IPV6_H */