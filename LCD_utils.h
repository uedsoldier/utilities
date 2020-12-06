/**
 * Definición de caracteres especiales y funciones para display LCD basadas en el controlador HD44780 de Hitachi y similares,
 * como los drivers KS0066 y S6A0069 de Samsung, ya sea para su manejo en interfaz parlela directamente o mediante expansor i2c-paralelo
 */

/**
 * Definición de caracteres griegos y especiales
*/
// α - alfa
#define LCD_alpha       0xE0  
// β - beta
#define LCD_beta        0xE2
// ϵ - epsilon
#define LCD_epsilon     0xE3
// μ - mu
#define LCD_mu          0xE4
// Ω - Omega
#define LCD_OMEGA       0xF4
// σ - sigma
#define LCD_sigma       0xE5
// Σ - Sigma
#define LCD_SIGMA       0xF6
// ρ - ro
#define LCD_rho          0xE6
// π - pi
#define LCD_pi          0xF7
// θ - theta
#define LCD_theta       0xF2
// ° - símbolo de grados
#define LCD_degree      0xDF
// ñ 
#define LCD_enie        0xEE
// Ñ
#define LCD_ENIE        0xEE
// ä
#define LCD_a_umlaut    0xE1
// ö
#define LCD_o_umlaut    0xEF
// ü
#define LCD_u_umlaut    0xF5
// ∞
#define LCD_infinity    0xF3
// Potencia a la menos uno (-1)
#define LCD_pot_neg     0xE9

/**
 * Macro de definición de caracteres por renglón. Adaptar según display (16 o 20)
 */
#define LCD_CHARS_PER_ROW 20

/**
 * Macro de definición de renglones. Adaptar según display (2 o 4)
 */
#define LCD_TOTAL_ROWS 4

/**
 * Direcciones DDRAM del módulo LCD
 */
#define LCD_ROW0_DIR  0x00	// Linea 0 de la pantalla
#define LCD_ROW1_DIR  0x40	// Linea 1 de la pantalla
#define LCD_ROW2_DIR  0x14	// Linea 2 de la pantalla
#define LCD_ROW3_DIR  0x54	// Linea 3 de la pantalla

/**
 * Constantes de comando y comandos de posicionamiento a utilizar por función LCD_command
 */

/**
 * Limpieza de display y reinicio a dirección 0 de DDRAM 
 */
#define LCD_CLEAR        		0x01

/**
 * Regreso a posición inicial de dirección DDRAM
 */
#define LCD_HOME         		0x02

/**
 * Establecimiento de dirección de cursor (incremental o decremental)
 */
#define LCD_CURSOR_DEC   		0x04
#define LCD_CURSOR_INC   		0x06

/**
 * Control ON-OFF de display:
 * - Display ON-OFF
 * - Cursor ON-OFF
 * - Parpadeo de cursor (Blink) ON-OFF
 */
#define LCD_OFF          		0x08
#define LCD_ON 					0x0C
#define LCD_CURSOR_ON    		0x0E
#define LCD_CURSOR_OFF   		0x0C
#define LCD_BLINK_ON     		0x0F
#define LCD_BLINK_OFF    		0x0E

/**
 * "Shift" de cursor y de display
 */
#define LCD_SHIFT_CURSOR_LEFT	0x10
#define LCD_SHIFT_CURSOR_RIGHT 	0x14
#define LCD_SHIFT_DISPLAY_LEFT	0x18
#define LCD_SHIFT_DISPLAY_RIGHT	0x1C

/**
 * Establecimiento de función:
 * - Interfaz de 4(0) u 8(1) bits
 * - Número de líneas: 1(0) o múltiples(1)
 * - Tipo de caracter: 5x7(0) o 5x10(1)
 */
// Interfaz de 4 bits
#define LCD_4_BIT			0x20 
// Interfaz de 8 bits 
#define LCD_8_BIT			0x30 	
// Display de una línea
#define LCD_1_LINE  		0x20 	
// Display multilínea
#define LCD_MULTILINE 		0x28 	
// Caracteres de 5x8 puntos
#define LCD_CHARS_5X8		0x20 
// Caracteres de 5x10 puntos
#define LCD_5X10			0x24  

// Dirección DDRAM (Data Direction RAM)
#define LCD_SET_DDRAM		0x80
// Dirección fila 0
#define LCD_ROW0            (LCD_SET_DDRAM|LCD_ROW0_DIR)
// Dirección fila 1
#define LCD_ROW1            (LCD_SET_DDRAM|LCD_ROW1_DIR)
// Dirección fila 2
#define LCD_ROW2            (LCD_SET_DDRAM|LCD_ROW2_DIR)
// Dirección fila 3
#define LCD_ROW3            (LCD_SET_DDRAM|LCD_ROW3_DIR)

// Comando para acceder a CGRAM (Character Generator RAM)
#define LCD_SET_CGRAM 		0x40

#define LCD_DEFAULT_CONFIG  (LCD_4_BIT|LCD_MULTILINE|LCD_CHARS_5X8)   // Configuración por defecto

/**
 * Caracteres personalizables almacenados en ROM: dar un vistazo a: www.quinapalus.com/hd44780udg.html.
 */

// Letra 'a' minúscula con acento
const char a_acento[] = {0x02, 0x04, 0x0E, 0x01, 0x0F, 0x11, 0x0F, 0x00};

// Letra 'e' minúscula con acento
const char e_acento[] = {0x02, 0x04, 0x0E, 0x11, 0x1F, 0x10, 0x0E, 0x00};

// Letra 'i' minúscula con acento
const char i_acento[] = {0x02, 0x04, 0x00, 0x0C, 0x04, 0x04, 0x0E, 0x00};

// Letra 'o' minúscula con acento
const char o_acento[] = {0x02, 0x04, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00};

// Letra 'u' minúscula con acento
const char u_acento[] = {0x02, 0x04, 0x11, 0x11, 0x11, 0x13, 0x0D, 0x00};

// Corazón vacío
const char corazon_vacio[] = {0,0,10,21,17,10,4,0};

// Corazón lleno
const char corazon_lleno[] = {0,0,10,31,31,14,4,0};

// Flecha de regreso
const char back[] = {28,2,1,5,9,30,8,4};

// Flecha arriba 1
const char arriba_1[] = {0,4,14,21,4,4,4,4};
// Flecha abajo 1
const char abajo_1[] = {0,4,4,4,4,21,14,4};

// Subíndice h
const char h_subindice[] = {0,0,0,0,16,16,28,20};

// Agregar casos según se requiera...
//...

/**
 * Prototipos de funciones 
 */
uint8_t LCD_CGROM(uint8_t dato);
void LCD_createChar(uint8_t address, const char *datos);


