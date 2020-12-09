/**
 * @file utils.c
 * @brief Librería de funciones, macros y utilidades en general para programación de microcontroladores PIC de 8 bits.
 * @author Ing. José Roberto Parra Trewartha
*/
#include <xc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "utils.h"

/**
 * @brief Función de retardos largos en milisegundos. A grandes frecuencias de oscilación,
 * es posible que la función __delay_ms(), desborde sus variables internas y arroje un error el compilador.
 * @param ms (uint16_t) Tiempo de retardo en milisegundos, desde 1 hasta 65535, el retardo no es preciso debido
 * a las operaciones internas.
 * @return (void)
*/
void delay_ms(uint16_t ms) {
	for(;ms;ms--)
		__delay_ms(1);
}

/**
 * @brief Función para invertir los bits de un dato (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 8 bits a ser invertido, ej: 001001101
 * @return dato de 8 bits invertido,                    ej: 101100100
*/
uint8_t invierte_bitsByte(uint8_t dato_original) {
	uint8_t dato_invertido = 0;
	for( uint8_t i=0 ; i != 8; i++) {
		if ( (dato_original & (1<<i)) != 0)
			dato_invertido|= (1 << (7-i));
	}
	return dato_invertido;
}

/**
 * @brief Función para invertir los bits de un dato de 16 bits (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 16 bits a ser invertido 
 * @return dato de 16 bits invertido,                     
*/
uint16_t invierte_bitsInt16(uint16_t dato_original) {
    uint16_t dato_invertido = 0;
    for(uint8_t i=0 ; i != 16; i++)
    {
        if ( (dato_original & (1<<i)) != 0)
            dato_invertido|= (1 << (15-i));
    }
    return dato_invertido;
}

/**
 * @brief Función para invertir los bits de un dato de 32 bits (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 32 bits a ser invertido 
 * @return dato de 32 bits invertido,                     
*/
uint32_t invierte_bitsInt32(uint32_t dato_original) {
    uint32_t dato_invertido = 0;
    for(uint8_t i=0 ; i != 32; i++)
    {
        if ( (dato_original & (1<<i)) != 0)
            dato_invertido|= (1 << (31-i));
    }
    return dato_invertido;
}

/*
 * Funciones para codificación/decodificación Gray
*/

/**
 * @brief Función de conversión de dato binario a código Gray.
 * @param param dato_bin dato binario a codificar.
 * @return dato codificado.
*/
uint16_t Gray_encode (uint16_t dato_bin) {
	return dato_bin ^ (dato_bin >> 1);
}

/**
 * @brief Función de conversión de dato en código Gray a binario.
 * @param dato_gray (uint16_t) dato en código Gray.
 * @return dato decodificado.
*/
uint16_t Gray_decode (uint16_t dato_gray) {
	uint16_t dato_gray_nuevo = dato_gray; 
	while ( dato_gray >>= 1)
		dato_gray_nuevo ^= dato_gray;
	return dato_gray_nuevo;
}

/**
 * @brief Función para  inversión de nibbles dentro de un byte 
 * @param dato (uint8_t) Byte cuyos nibbles serán invertidos
 * @return (uint8_t) Byte con nibbles invertidos
*/
uint8_t nibble_swap(uint8_t dato) {
	return (dato << 4) | (dato >> 4);
}

/**
 * @brief Función de conversión de dato binario en un dato BCD (Binary Coded Decimal)
 * @param dato (uint8_t) Dato binario a ser convertido a BCD
 * @return (uint8_t ) Dato BCD resultado
*/
uint8_t bin2bcd(uint8_t dato_bin) {
    uint8_t retval=0,dato_aux;
    dato_aux=dato_bin;
    if(dato_aux<10)
        return dato_aux;
    else {
        do{
            dato_aux-=10;
            retval+=0x10;
        }
        while(dato_aux>=10);
        retval+=dato_aux;
        return retval;
    }
}

/**
 * @brief Función de conversión de dato BCD (Binary Coded Decimal) en un dato binario
 * @param dato (uint8_t) Dato BCD a ser convertido a binario
 * @return (uint8_t ) Dato binario resultado
*/
uint8_t bcd2bin(uint8_t dato_bcd) {
    uint8_t temp_h,dato;
    if(dato_bcd<0x10)
        return dato_bcd;
    else {
        temp_h=(dato_bcd>>4);
        dato=(dato_bcd&0x0F);
        do {
            temp_h--;
            dato+=10;
        }
        while(temp_h!=0);
        return dato;
    }
}

/**
 * @brief Función para mapeo de valores enteros de un dominio al otro en interpolación lineal.
 * @param x (int16_t): Valor a interpolar en el otro dominio
 * @param x0 (int16_t): Valor inferior del dominio inicial
 * @param x1 (int16_t): Valor superior del sominio inicial
 * @param y0 (int16_t): Valor inferior del dominio final 
 * @param y1 (int16_t): Valor superior del dominio final
 * @return (in16_t) Valor correspondiente interpolado
 */
int16_t map(int16_t x, int16_t x0, int16_t x1, int16_t y0, int16_t y1){
    return (x-x0) * ( (y1-y0)/(x1-x0) ) + y0;
} 


/**
 * @brief Función para realización de división de enteros de 32 bits sin signo. Muy útil en cálculos para registros, ya que no realiza truncamiento sino redondeo al entero más cercano
 * @param (uint32_t) Dividendo 
 * @param (uint32_t) Divisor
 * @return (uint32_t) Cociente
*/
uint32_t division_entera_sin_signo(uint32_t dividendo, uint32_t divisor) {
    return ((dividendo+(divisor>>1))/divisor);
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como entero de 32 bits, con 2 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (int32_t) Equivalente entero con 2 decimales
*/
int32_t int32_00(whole_frac_t *dato) {
    return ((int32_t)dato->whole) * 100L + dato->frac; 
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como entero de 16 bits, con 2 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (int16_t) Equivalente entero con 2 decimales
*/
int16_t int16_00(whole_frac_t *dato)  { 
    return dato->whole * 100 + dato->frac; 
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como entero de 32 bits, con 3 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (int32_t) Equivalente entero con 3 decimales
*/
int32_t int32_000(whole_frac_t *dato)  {
    return dato->whole * 1000L + dato->frac; 
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como float, con 2 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (float) Equivalente flotante con 2 decimales
*/
float float_00(whole_frac_t *dato)  { 
    return ((float)dato->whole) + ((float)dato->frac)*0.01; 
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como float, con 3 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (int32_t) Equivalente flotante con 3 decimales
*/
float float_000(whole_frac_t *dato)  { 
    return ((float)dato->whole) + ((float)dato->frac)*0.001; 
}

/**
 * @brief Función de inicialización de estructura de datos de tipo entero/fraccionario. Inicialización de ambas partes en cero
 * @param dato (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (void)
 */
void whole_frac_init(whole_frac_t *dato) {
    dato->frac = 0;
    dato->whole = 0;
}


/**
 * @brief Función que localiza un caracter o cadena dentro de otra cadena. Ejemplo: 
 * cadena_principal = "Hola mundo"
 * Posiciones: 
 * 'H' 'o' 'l' 'a' ' ' 'm' 'u' 'n' 'd' 'o' '\\0'
 *  0   1   2   3   4   5   6   7   8   9   10
 * Cadena_a_buscar = "mundo"
 * Resultado de la función: 5
 * @param cadena_principal: (const char*) cadena de caracteres en la cual se buscará 
 * @param cadena_a_buscar: (const char*) cadena de caracteres cuya ocurrencia se buscará en la cadena principal
 * @return posicion: (int16) Índice de ocurrencia de la cadena a buscar en la cadena principal. Si no hay ocurrencia se devuelve -1
*/
int16_t string_indexOf( const char *cadena_a_buscar, const char *cadena_principal) {
    char *apuntador_ocurrencia = strstr(cadena_principal,cadena_a_buscar);
    if(apuntador_ocurrencia == NULL)
        return -1;
    else
        return (int16_t)(apuntador_ocurrencia - cadena_principal);
}   

/**
 * @brief Función que calcula valor de precarga de timer0 para generar un timeout vía la bandera de interrupción por desborde (TMR0IF).
 * El usuario deberá configurar al timer 0 y después habilitar manualmente el timer0 (T0CONbits.TMR0ON = 1), para dar mayor exactitud a la temporización.
 * @param timeout_ms: (timeout_ms) Timeout en milisegundos requerido 
 * @return (void) 
*/
void set_timeOut_ms(uint16_t timeout_ms) {
    TMR0ON = 0;     // Si el timer está activado, desactívalo
    timeout_ns = ((uint32_t)(timeout_ms)) * 1000000UL;
    precarga_timer0 = (uint16_t)(65536UL - (uint16_t)(division_entera_sin_signo(timeout_ns, 256UL * TCY_ns )));     //Cálculo de precarga para timer 0
    TMR0H = make8(precarga_timer0,1);
	TMR0L = make8(precarga_timer0,0);
    TMR0IF = 0;     //Limpia bandera de interrupción por desborde de timer 0
}

/**
 * @brief Función para obtención de tiempo transcurrido en [ns], ante una condición predefinida por el usuario, sin que se active la
 * bandera de interrupción por desborde (TMR0IF), y habiendo detenido el timer0 (T0CONbits.TMR0ON = 0).
 * @param (void) 
 * @return (uint32_t) Timeout en nanosegundos transcurrido. 
*/
uint32_t get_ellapsedTime_ns() {
    uint8_t tmr0l = TMR0L;		//Lectura de byte bajo para actualizar buffer de TMR0H 
    return ( make16(TMR0H,tmr0l) - precarga_timer0 ) * 256UL * TCY_ns; //Cálculo de tiempo de espera transcurrido en [ns]  
}

/**
 * @brief Función para obtención de tiempo transcurrido en [us], ante una condición predefinida por el usuario, sin que se active la
 * bandera de interrupción por desborde (TMR0IF), y habiendo detenido el timer0 (T0CONbits.TMR0ON = 0).
 * @param (void) 
 * @return (float) Timeout en microsegundos transcurrido. 
*/
float get_ellapsedTime_us() {
    return ((float)(get_ellapsedTime_ns()))/1000.0F;            //Cálculo de tiempo de espera transcurrido en [us]
}

/**
 * @brief Función para obtención de tiempo transcurrido en [us], ante una condición predefinida por el usuario, sin que se active la
 * bandera de interrupción por desborde (TMR0IF), y habiendo detenido el timer0 (T0CONbits.TMR0ON = 0).
 * @param (void) 
 * @return (float) Timeout en milisegundos transcurrido. 
*/
float get_ellapsedTime_ms() {
    return ((float)(get_ellapsedTime_ns()))/1000000.0F;         //Cálculo de tiempo de espera transcurrido en [ms]
}


