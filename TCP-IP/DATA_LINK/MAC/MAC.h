#ifndef MAC_H
#define MAC_H

#pragma region Dependencies
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../../utils.h"
#pragma endregion

#pragma region Useful macros
/**
 * @brief Macro para activar o desactivar logging por puerto serie
 */
#ifndef MAC_LOG
#define	MAC_LOG	0
#endif

#define MAC_LOWERCASE false
#define MAC_UPPERCASE true

#define MAC_SIZE_BYTES 6

/**
 * @brief 
 * 
 */
#define MAC_MAX_SIZE 18
#pragma endregion

#pragma region Custom types
/**
* @brief Definición de estructura de datos para direccionamiento MAC
*/
typedef union MAC_address_t {
    uint8_t MAC_array[MAC_SIZE_BYTES];
    struct {
        uint8_t b0, b1, b2, b3, b4, b5;
    } MAC_bytes;
    struct {
        uint16_t w0, w1, w2;
    } MAC_words;
} MAC_address_t;

/**
 * @brief 
 * 
 */
typedef enum MAC_error {
    MAC_NULL_STRING, MAC_NULL_TOKEN ,MAC_NaN, MAC_INVALID_NUMBER, MAC_INVALID_ADDRESS, MAC_INVALID_ARRAY_LENGTH ,MAC_ADDRESS_OK
} MAC_error_t;
#pragma endregion

#pragma region Function prototypes
MAC_error_t MAC_fromArray(MAC_address_t *address, uint8_t *bytes);
void array_fromMAC(MAC_address_t *address, uint8_t *bytes);
MAC_error_t MAC_fromString(MAC_address_t *address, char *string);
char *string_fromMAC(MAC_address_t *address, bool upper);
#pragma endregion

#endif /*MAC_H*/