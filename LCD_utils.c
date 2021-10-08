#include <stdint.h>
#include "LCD_utils.h"

/**
 * @brief Función de búsqueda de caracteres en la CGROM del display (caracteres predefinidos)
 * @param dato (uint8_t) Caracter buscado
 * @return (uint8_t) Byte con el valor correcto de CGROM para mostrar el caracter deseado
*/
uint8_t LCD_CGROM(uint8_t dato) {
    switch(dato) {
        case 164: //''
            return LCD_enie;
        case 165: //'Ñ'
            return LCD_ENIE;
        case 167: //'°'
            return LCD_degree;
        case 129: //'ü'
            return LCD_u_umlaut;
        case 132: //'ä'
            return LCD_a_umlaut;
        case 148: //'ö'
            return LCD_o_umlaut;
        default: return dato;   // i no está el caracter en la CGROM, se considera un caracter convencional ASCII
    }
}