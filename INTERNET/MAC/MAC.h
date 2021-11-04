#ifndef MAC_H
#define MAC_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Macro para activar o desactivar logging por puerto serie
 */
#define	MAC_LOG	1

/**
* @brief Definición de estructura de datos para direccionamiento MAC
*/
typedef union MAC_address_t {
    uint8_t MAC_array[6];
    struct {
        uint8_t b0, b1, b2, b3, b4, b5;
    } MAC_bytes;
    struct {
        uint16_t w0, w1, w2;
    } MAC_words;
} MAC_address_t;

// Funciones de utilidad de direccionamiento MAC
void makeMACfromArray(MAC_address_t *address, uint8_t *bytes);
void makeArrayFromMAC(MAC_address_t *address, uint8_t *bytes);
void makeMACfromString(MAC_address_t *address, char *string);
char *makeStringFromMAC(MAC_address_t *address);

#endif /*MAC_H*/