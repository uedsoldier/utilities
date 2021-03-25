/**
 * @file utils.h
 * @brief Librería de funciones, macros y utilidades en general para programación de microcontroladores PIC de 8 bits.
 * @author Ing. José Roberto Parra Trewartha
*/
#ifndef UTILS_H

#define UTILS_H

/*
	Macros que definen la frecuencia de oscilación principal en [Hz], [kHz] y el ciclo máquina en [ns]
	Se realiza en este sitio para no tener que definirla en todos los archivos .c
	Estos archivos deberán incluir al header utils.h para que esto se cumpla
*/

/**
 * @brief Macro de definición de frecuencia principal en [Hz]
*/
#if !defined(_XTAL_FREQ)
#define _XTAL_FREQ 64000000UL
#endif

/**
 * @brief Macro de definición de frecuencia principal en [kHz]. Calculada a partir de la macro _XTAL_FREQ
*/
#define _XTAL_FREQ_kHz		(_XTAL_FREQ/1000UL)	

/**
 * @brief Macro de definición de frecuencia principal en [MHz]. Calculada a partir de la macro _XTAL_FREQ_kHz
*/
#define _XTAL_FREQ_MHz		(_XTAL_FREQ_kHz/1000UL)

/**
 * @brief Macro de definición de tiempo de ciclo máquina en [ns]. Calculada a partir de la macro _XTAL_FREQ_kHz
*/
#define TCY_ns 	(4000000UL/_XTAL_FREQ_kHz)

/**
 * @brief Macro de definición de tiempo de ciclo máquina en [ps]. Calculada a partir de la macro _XTAL_FREQ_MHz
*/
#define TCY_ps 	(4000000UL/_XTAL_FREQ_MHz)

/**
 * Macros para eliminación de warnings del compilador, comentar para fines de depuración.
*/
#pragma warning disable 520     // function * is never called
#pragma warning disable 373     // implicit signed to unsigned conversion
#pragma warning disable 343     // implicit return at end of non-void function
#pragma warning disable 1498    // pointer (*) in expression may have no targets 
#pragma warning disable 2053    // function X is never called
#pragma warning disable 759     // expression generates no code
#pragma warning disable 751     // arithmetic overflow in constant expression

/*
	Macros para manejo individual de bits dentro variables. El parámetro bit indica la posición, que 
	empieza desde el bit 0 y el último bit será 7 (1 byte), 15 (2 bytes) o 31(4 bytes)
	Validada 8-10-2017 (ya había sido validadas desde proyecto de tesis CCA). Las macros de 32 bits fueron 
    validadas el 28-12-2018.
*/

/**
 * @brief Macro para intercambio de variables sin variables intermedias
 * @param a
 * @param b
 * @return (void)
*/
#define swap(a,b) (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b)))

/**
 * @brief Macro que devuelve el valor enmascarado de un bit
 * @param bit: Número de bit enmascarado
 * @return (bool) Valor del bit enmascarado
*/
#define _bv(bit) (1 << (bit))

/**
 * @brief Macro que devuelve el valor de un bit dentro de una variable
 * @param var: Variable a leer
 * @param bit: Número de bit a leer
 * @return (bool) Valor del bit dentro de la variable
*/
#define testbit(var,bit) ((var)&(1<<(bit))) 	
/**
 * @brief Macro que pone en alto un bit de una variable
 * @param var: Variable a escribir
 * @param bit: Número de bit a poner en alto
 * @return (void)
*/
#define setbit(var,bit) ((var)|=(1<<(bit))) 	
/**
 * @brief Macro que pone en bajo un bit de una variable
 * @param var: Variable a escribir
 * @param bit: Número de bit a poner en bajo
 * @return (void)
*/
#define clrbit(var,bit) ((var)&=~(1<<(bit))) 
/**
 * @brief Macro para cambiar de estado un bit de una variable
 * @param var: Variable a escribir
 * @param bit: Número de bit a cambiar de estado
 * @return (void)
*/
#define togglebit(var,bit) ((var)^=(1<<(bit)))	

/**
 * @brief Macro que devuelve el valor de un bit dentro de una variable de 32 bits
 * @param var: Variable a leer
 * @param bit: Número de bit a leer
 * @return (bool) Valor del bit dentro de la variable de 32 bits
*/
#define testbit32(var,b) ((((uint32_t)var)&(0x00000001UL<<((uint8_t)b)))? true:false)
/**
 * @brief Macro que pone en alto un bit de una variable de 32 bits
 * @param var: Variable a escribir
 * @param bit: Número de bit a poner en alto
 * @return (void)
*/
#define setbit32(var,b) (((uint32_t)var)|=(0x00000001UL<<((uint8_t)b))) 
/**
 * @brief Macro que pone en bajo un bit de una variable de 32 bits
 * @param var: Variable a escribir
 * @param bit: Número de bit a poner en bajo
 * @return (void)
*/
#define clrbit32(var,b) (((uint32_t)var)&=~(0x00000001UL<<((uint8_t)b))) 
/**
 * @brief Macro para cambiar de estado un bit de una variable de 32 bits
 * @param var: Variable a escribir
 * @param bit: Número de bit a cambiar de estado
 * @return (void)
*/
#define togglebit32(var,b) (((uint32_t)var)^=(0x00000001UL<<((uint8_t)b)))	

/**
 * @brief Macro para obtención de bytes que componen a una variable de 2 bytes o más. El Byte 0 es el menos significativo, 
 * mientras que el más significativo puede ser el Byte 1 (variable de 16 bits) o el Byte 3 (variable de 32 bits).
 * Validada 8-10-2017
 * @param var: Variable
 * @param byte: Número de byte a obtener
 * @return (uint8_t) Valor del byte deseado
*/
#define make8(var,byte) ((var>>(8*byte))&0xFF)

/**
 * @brief Macro para crear una variable de 16 bits a partir de bytes individuales B0 y B1.
 * Validada 8-10-2017
 * @param B1
 * @param B0
 * @return (uint16_t) Variable de 16 bits creada
*/
#define make16(B1,B0) (((uint16_t)B1<<8)|(uint16_t)B0)

/**
 * @brief Macro para crear una variable de 32 bits a partir de bytes individuales B0, B1, B2 y B3.
 * Validada 8-10-2017
 * @param B3
 * @param B2
 * @param B1
 * @param B0
 * @return (uint32_t) Variable de 32 bits creada
*/
#define make32(B3,B2,B1,B0) (((uint32_t)B3<<24)|((uint32_t)B2<<16)|((uint32_t)B1<<8)|(uint32_t)B0)

/**
 * Macros para imprimir o enviar valores binarios
*/
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
	(byte & 0x80 ? '1' : '0'), \
	(byte & 0x40 ? '1' : '0'), \
	(byte & 0x20 ? '1' : '0'), \
	(byte & 0x10 ? '1' : '0'), \
	(byte & 0x08 ? '1' : '0'), \
	(byte & 0x04 ? '1' : '0'), \
	(byte & 0x02 ? '1' : '0'), \
	(byte & 0x01 ? '1' : '0') 

/*
	Macros y estructuras de datos para operaciones matemáticas
*/

/**
 * @brief Macro para restricción de valores a límites inferior y superior (constrain)
 */
#define constrain(x,inf,sup) ((x > sup)? sup:((x < inf)? inf: x ))

/**
 * @brief Definición de tipo de estructura de datos para manejo de aritmética de punto fijo, con partes enteras y fraccionarias
*/
typedef struct whole_frac_t {
	int16_t whole;
    int16_t frac;
} whole_frac_t;

/*
	Macros y funciones para utilización con arreglos
*/
bool array_compare(const void *array1, const void *array2, uint16_t len);

/**
 * @brief Macro que devuelve la cantidad de bytes en un arreglo. Se divide el número de bytes
 * totales del arreglo entre el número de bytes de uno de los elementos.
 * @param array: Arreglo de datos cualquier tipo
 * @return Tamaño del arreglo en bytes
*/
#define array_length(array)	(sizeof(array)/sizeof(array[0]))

/**
 * Prototipos de funciones
*/

//Función de retardo en milisegundos
void delay_ms(uint16_t milisegundos);

//Manejo de datos a nivel bit

uint8_t invierte_bitsByte(uint8_t dato_original);		// Inversión de bits en un dato de 8 bits
uint16_t invierte_bitsInt16(uint16_t dato_original);	// Inversión de bits en un dato de 16 bits	
uint32_t invierte_bitsInt32(uint32_t dato_original);	// Inversión de bits en un dato de 32 bits	
uint8_t nibble_swap(uint8_t dato);						// Intercambio de nibbles dentro de un byte

//Conversión de datos

uint16_t Gray_encode (uint16_t dato_bin);		// Codificador Gray
uint16_t Gray_decode (uint16_t dato_gray);		// Decodificador Gray
uint8_t bin2bcd(uint8_t dato_bin);				// Dato binario a BCD (Binary Coded Decimal)
uint8_t bcd2bin(uint8_t dato_bcd);				// Dato BCD (Binary Coded Decimal) a binario
uint8_t ascii2byte(uint8_t caracter);           // Dato ASCII a byte
uint8_t byte2ascii(uint8_t caracter);           // Byte a dato ASCII 

//Operaciones matemáticas
int16_t map(int16_t x, int16_t x0, int16_t x1, int16_t y0, int16_t y1);	//Mapeo de valores de un dominio a otro 
uint32_t division_entera_sin_signo(uint32_t dividendo, uint32_t divisor);	// División de enteros sin truncamiento

// Aritmética de punto fijo
int32_t int32_00(whole_frac_t *dato);			// Estructura entero/fraccionario (2 decimales) a entero equivalente de 32 bits 
int16_t int16_00(whole_frac_t *dato); 			// Estructura entero/fraccionario (2 decimales) a entero equivalente de 16 bits 
int32_t int32_000(whole_frac_t *dato);  		// Estructura entero/fraccionario (3 decimales) a entero equivalente de 32 bits 
float float_00(whole_frac_t *dato);  			// Estructura entero/fraccionario (2 decimales) a flotante equivalente (24 o 32 bits en el caso de Microchip) 
float float_000(whole_frac_t *dato); 			// Estructura entero/fraccionario (3 decimales) a flotante equivalente (24 o 32 bits en el caso de Microchip)
void whole_frac_init(whole_frac_t *dato);		// Inicialización de estructura de datos entero/fraccionario (whole_frac)

// Manejo de cadenas
int16_t string_indexOf( const char *cadena_a_buscar, const char *cadena_principal);


/**
 * Prototipo de funciones y variables para generación de timeouts, utilizando timer0
*/
void set_timeOut_us(uint16_t timeout_us);
void set_timeOut_ms(uint16_t timeout_ms);
void set_prescaler_timeout(uint16_t prescale);

//Funciones para obtener tiempo transcurrido antes de que ocurriera timeout
uint32_t get_ellapsedTime_ns(void);	// Tiempo transcurrido en [ns] 
float get_ellapsedTime_us(void);	// Tiempo transcurrido en [μs]
float get_ellapsedTime_ms(void);    // Tiempo transcurrido en [ms]     

volatile uint32_t timeout_ns;            //Valor de timeout en [ns]
volatile uint16_t precarga_timer0;       //Variable de precarga para timer 0

/**
 * Prototipos de funciones y variables para conteo de milisegundos, utilizando timer0
*/
void millisecond_counter_init();
void millisecond_counter_callback();
uint16_t millisecond_counter_get();
void millisecond_counter_reset();

volatile static uint16_t milliseconds_count;

#endif /*UTILS_H*/