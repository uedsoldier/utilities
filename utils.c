/**
 * @file utils.c
 * @author Roberto Parra (uedsoldier1990@gmail.com)
 * @brief Librería de funciones, macros y utilidades en general para programación de microcontroladores primordialmente
 * de bajas prestaciones, aunque se puede adaptar a otros sin mayor detalle.
 * @version 0.1
 * @date 2022-03-28 
 */

#include "utils.h"

/**
 * @brief Función que compara uno a uno 'len' elementos de dos arreglos de datos o estructuras.
 * @param array1 (const void*) Arreglo de datos cualquier tipo
 * @param array2 (const void*) Arreglo de datos cualquier tipo
 * @param len (size_t) Cantidad de elementos a comparar
 * @return (bool) True en caso de que ambos arreglos sean iguales en sus primeros 'len' elementos
 */
bool array_compare(const void *array1, const void *array2, size_t len){
    uint8_t *_array1 = (uint8_t*)array1;
    uint8_t *_array2 = (uint8_t*)array2;
    bool retVal = true; 
    for(size_t i = 0; i != len; i++) {
        if(_array1[i] != _array2[i]) {
            retVal = false;
            break;
        }
    }
    return retVal;
}

/**
 * @brief Función que comprueba si los primeros 'len' elementos de un arreglo son todos ceros
 * @param array (const void*) Arreglo de datos cualquier tipo
 * @param len (size_t) Cantidad de elementos a comparar
 * @return (bool) True en caso de que todos lo primeros 'len' elementos del arreglo sean ceros
 */
bool array_isAllZeros(const void *array, size_t len){
    uint8_t *_array = (uint8_t*)array;
    for(size_t i = 0; i != len; i++) {
        if(_array[i] != 0) 
            return false;
    }
    return true;
}


/**
 * @brief Función de retardos largos en milisegundos. A grandes frecuencias de oscilación,
 * es posible que la función __delay_ms(), desborde sus variables internas y arroje un error el compilador.
 * @param ms (uint16_t) Tiempo de retardo en milisegundos, desde 1 hasta 65535, el retardo no es preciso debido
 * a las operaciones internas.
 * @return (void)
*/
void delay_ms(uint16_t ms) {
	for(;ms;ms--) {
        #if defined(__XC8)
		__delay_ms(1);
        #else

        #endif
    }
}

/**
 * @brief Función para invertir los bits de un dato (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 8 bits a ser invertido, ej: 001001101
 * @return (uint8_t) dato de 8 bits invertido,          ej: 101100100
*/
uint8_t bit_invert_Byte(uint8_t dato_original) {
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
 * @return (uint16_t) dato de 16 bits invertido,                     
*/
uint16_t bit_invert_Int16(uint16_t dato_original) {
    uint16_t dato_invertido = 0;
    for(uint8_t i=0 ; i != 16; i++)
    {
        if ( (dato_original & (1<<i)) != 0)
            dato_invertido|= (1 << (15-i));
    }
    return dato_invertido;
}

#if defined(__XC8)
/**
 * @brief Función para invertir los bits de un dato de 24 bits (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 24 bits a ser invertido 
 * @return (uint24_t)dato de 24 bits invertido,                     
*/
uint24_t bit_invert_Int24(uint24_t dato_original) {
    uint24_t dato_invertido = 0;
    for(uint8_t i=0 ; i != 24; i++)
    {
        if ( (dato_original & (1<<i)) != 0)
            dato_invertido|= (1 << (23-i));
    }
    return dato_invertido;
}
#endif

/**
 * @brief Función para invertir los bits de un dato de 32 bits (útil para mandar/recibir LSb primero) 
 * @param dato_original dato de 32 bits a ser invertido 
 * @return dato de 32 bits invertido,                     
*/
uint32_t bit_invert_Int32(uint32_t dato_original) {
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
 * @brief Función para la inversión de nibbles dentro de un byte 
 * @param dato (uint8_t) Byte cuyos nibbles serán invertidos
 * @return (uint8_t) Byte con nibbles invertidos
*/
uint8_t nibble_swap(uint8_t dato) {
	return (uint8_t)(dato << 4) | (dato >> 4);
}

/**
 * @brief Función para la inversión de bytes dentro de una variable de 2 bytes
 * @param dato (uint16_t) Variable de dos bytes cuyos bytes serán invertidos
 * @return (uint16_t) Variable de dos bytes con bytes invertidos
*/
uint16_t byte_swap(uint16_t dato) {
	return ((dato & (uint16_t)0xFF00) >> 8) | ((dato & (uint16_t)0x00FF) << 8);
}

/**
 * @brief Función para la inversión de bytes dentro de una variable de 4 bytes
 * @param dato (uint32_t) Variable de cuatro bytes cuyos words serán invertidos
 * @return (uint32_t) Variable de dos cuatro con words invertidos
*/
uint32_t word_swap(uint32_t dato) {
	return ((dato & (uint32_t)0xFFFF0000) >> 16) | ((dato & (uint32_t)0x0000FFFF) << 16);
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
 * @brief Funcion que convierte un caracter ASCII numerico a su valor correspondiente como byte
 * @param caracter (uint8_t) Dato ASCII a convertir en byte
 * @return (uint8_t) Byte
 */
uint8_t ascii2byte(uint8_t caracter) {               

    if(caracter>=0x30 && caracter<=0x39){ //Si es numero conviertelo
    
        return caracter-0x30; //Regresalo como numero
    }
    else
        return 0;
}

/**
 * @brief Funcion que convierte un byte a su valor correspondiente como caracter ASCII numerico 
 * @param byte (uint8_t) Byte a convertir en dato ASCII
 * @return (uint8_t) Dato ASCII
 */
uint8_t byte2ascii(uint8_t byte){           
    if(byte <= 9){ //Si es numero conviertelo
    
        return byte+0x30; //Regresalo como ASCII
    }
    else
        return 0;
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
    return ((float)dato->whole) + ((float)dato->frac)*0.01F; 
}

/**
 * @brief Función de conversión de estructura de datos de tipo entero/fraccionario a su equivalente como float, con 3 decimales.
 * @param (whole_frac_t*) Apuntador a estructura de datos del tipo entero/fraccionario (whole_frac)
 * @return (int32_t) Equivalente flotante con 3 decimales
*/
float float_000(whole_frac_t *dato)  { 
    return ((float)dato->whole) + ((float)dato->frac)*0.001F; 
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
 * @brief Función para validar que una cadena de caracteres corresponde a una secuencia de dígitos
 * @param str 
 * @return true 
 * @return false 
 */
bool string_validate_int(const char *str){
    while (*str) {
        if(!isdigit(*str)){
            return false;
        }
        str++;
    }
    return true;
}

/**
 * @brief Función para validar que una cadena de caracteres corresponde a una secuencia de dígitos hexadecimales (0-9 y a-f)
 * 
 * @param str 
 * @return true 
 * @return false 
 */
bool string_validate_hex(const char *str) {
    while (*str) {
        if(!isxdigit(*str)){
            return false;
        }
        str++;
    }
    return true;
}

#if defined(__XC8)
/**
 * @brief Función que calcula valor de precarga de timer0 para generar un timeout vía la bandera de interrupción por desborde (TMR0IF).
 * El usuario deberá configurar previamente al timer 0 (preescala, fuente de reloj y modo de 16 bits) y después habilitar manualmente el timer0 (T0CONbits.TMR0ON = 1),
 * para dar mayor exactitud a la temporización. Se recomienda no tener activa la interrupción por desborde de timer0 (TMR0IE)
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
 * @brief Función que calcula valor de precarga de timer0 para generar un timeout vía la bandera de interrupción por desborde (TMR0IF).
 * El usuario deberá configurar previamente al timer 0 (preescala, fuente de reloj y modo de 16 bits) y después habilitar manualmente el timer0 (T0CONbits.TMR0ON = 1),
 * para dar mayor exactitud a la temporización.
 * @param timeout_us: (timeout_us) Timeout en microsegundos requerido 
 * @return (void) 
*/
void set_timeOut_us(uint16_t timeout_us){
    TMR0ON = 0;     // Si el timer está activado, desactívalo
    timeout_ns = ((uint32_t)(timeout_us)) * 1000UL;
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

/**
 * @brief Función que calcula valor de precarga de timer0 para generar conteo de milisegundos vía la bandera de interrupción por desborde (TMR0IF).
 * El usuario deberá configurar previamente al timer 0 (preescala, fuente de reloj y modo de 16 bits) y después habilitar manualmente el timer0 (T0CONbits.TMR0ON = 1),
 * para dar mayor exactitud a la temporización. El usuario deberá activar las banderas de habilitación de interrupciones
 * @param (void)
 * @return (void) 
 */
void millisecond_counter_init() {
    TMR0ON = 0;     // Si el timer está activado, desactívalo
    precarga_timer0 = (uint16_t)(65536UL - (uint16_t)(division_entera_sin_signo(1000000000UL, 256UL * TCY_ns )));     //Cálculo de precarga para timer 0
    TMR0H = make8(precarga_timer0,1);
	TMR0L = make8(precarga_timer0,0);
    TMR0IF = 0;     //Limpia bandera de interrupción por desborde de timer 0
    milliseconds_count = 0;    // Reinicio de contador de milisegundos
    TMR0IE = 1;     // Habilita interrupción por desborde de timer 0
}

/**
 * @brief Función para incrementar el contador de milisegundos. Se debe llamar en la función de interrupción, habiendo configurado correctamente el contador de milisegundos a
 * través de la función millisecond_counter_init()
 * @param (void)
 * @return (void) 
 */
void millisecond_counter_callback() {
    milliseconds_count++;  // Incremento de conteo de [ms]
}

/**
 * @brief Función para obtener el contador de milisegundos transcurridos
 * @param (void)
 * @return (uint16_t) Contador de [ms] transcurridos
 */
uint32_t millisecond_counter_get(){
    return milliseconds_count;
}

/**
 * @brief Función para reinicio de contador de milisegundos a los valores iniciales para desactivar esta funcionalidad.
 * @param (void)
 * @return (void)
 */
void millisecond_counter_reset() {
    TMR0ON = 0;                 // Desactiva timer0
    TMR0IE = 0;                 // Deshabilita interrupción por desborde de timer0
    milliseconds_count = 0;     // Reinicia contador de milisegundos
}

#endif



