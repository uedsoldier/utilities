/**
 * @file time.h
 * @brief Librería para manejo de estructuras de datos de tiempo
 * @author Ing. José Roberto Parra Trewartha
*/

#ifndef TIME_H
#define TIME_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "../utils.h"

/**
 * @brief 
 * 
 */
*/
#define TIME_LOG

/**
 * @brief Macro para habilitar / deshabilitar la modificación en tiempo de ejecución de la época. Si esto se define, los mutadores de época están disponibles.
 * Si esto no está definido, la época es una constante.
*/
#define TIME_EPOCH_MODIFIABLE

/**
 * @brief Definición de variable global de tiempo. Cantidad de segundos transcurridos desde el 1 de enero del año de la época, en
 * el caso del tiempo UNIX, 1970. 00:00:00 +0000 (UTC).
*/
typedef uint32_t  clock_t; 

/**
 * Macros de cálculos de tiempo
*/
#define SECONDS_PER_MINUTE 60		// 60 segundos por minuto
#define MINUTES_PER_HOUR   60		// 60 minutos por hora
#define SECONDS_PER_HOUR   3600		// 3600 segundos por hora
#define HOURS_PER_DAY      24		// 24 horas por día
#define SECONDS_PER_DAY    86400UL	// 86400 segundos por día
#define	DAYS_PER_WEEK      7		// 7 días por semana

/**
 * @brief Enumeración de días de la semana
*/
typedef enum weekday_t {
	SUNDAY = 1,MONDAY = 2,TUESDAY = 3,WEDNESDAY = 4,THURSDAY = 5,FRIDAY = 6,SATURDAY = 7		
} weekday_t;

/**
 * @brief Enumeración de meses del año
*/
typedef enum month_t {
	JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE, JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
} month_t;

// Base de tiempo NTP (Network Time Protocol) el día Lunes 1° de Enero de 1900
#define NTP_EPOCH_YEAR      1900        // Año época: 1900 NTP (Network Time Protocol)
#define NTP_EPOCH_WEEKDAY   MONDAY      // Inicio de época NTP el Lunes 1° de Enero de 1900

// Base de tiempo POSIX (o UNIX) el día Jueves 1° de Enero de 1970
#define POSIX_EPOCH_YEAR    1970        // Año época: 1970 POSIX
#define  POSIX_EPOCH_WEEKDAY THURSDAY   // Inicio de época POSIX el Jueves 1° de Enero de 1970

// Base de tiempo Y2K (en el año 2000) el día Sábado 1° de Enero del 2000
#define Y2K_EPOCH_YEAR      2000        // Año época: 1970 Y2K
#define Y2K_EPOCH_WEEKDAY   SATURDAY    // Inicio de época POSIX el Sábado 1° de Enero del 2000

/**
 * @brief Definición de estructura de datos de tiempo
*/
typedef struct time_t {
	uint8_t seconds;    // 00-59 Segundos
	uint8_t minutes;    // 00-59 Minutos
	uint8_t hours;      // 00-23 Horas
	uint8_t day;        // 01-07 Día de la semana
	uint8_t date;       // 01-31 Día del mes
	uint8_t month;      // 01-12 Mes
	uint8_t year;       // 00-99 Año
} time_t;

/**
 * @brief
 *
 */
typedef enum UTC_timezone_t
{
	UTC_n12_00 = 0,	 // Baker Island, Howland Island (both uninhabited)
	UTC_n11_00 = 1,	 // American Samoa, Samoa
	UTC_n10_00 = 2,	 // (Summer)French Polynesia (most), United States (Aleutian Islands, Hawaii)
	UTC_n09_30 = 3,	 // Marquesas Islands
	UTC_n09_00 = 4,	 // Gambier Islands;(Summer)United States (most of Alaska)
	UTC_n08_00 = 5,	 // (Summer)Canada (most of British Columbia), Mexico (Baja California)
	UTC_n08_00 = 6,	 // United States (California, most of Nevada, most of Oregon, Washington (state))
	UTC_n07_00 = 7,	 // Mexico (Sonora), United States (Arizona); (Summer)Canada (Alberta)
	UTC_n07_00 = 8,	 // Mexico (Chihuahua), United States (Colorado)
	UTC_n06_00 = 9,	 //  Costa Rica, El Salvador, Ecuador (Galapagos Islands), Guatemala, Honduras
	UTC_n06_00 = 10, // Mexico (most), Nicaragua;(Summer)Canada (Manitoba, Saskatchewan), United States (Illinois, most of Texas)
	UTC_n05_00 = 11, // Colombia, Cuba, Ecuador (continental), Haiti, Jamaica, Panama, Peru
	UTC_n05_00 = 12, // (Summer)Canada (most of Ontario, most of Quebec)
	UTC_n05_00 = 13, // United States (most of Florida, Georgia, Massachusetts, most of Michigan, New York, North Carolina, Ohio, Washington D.C.)
	UTC_n04_30 = 14, // Venezuela
	UTC_n04_00 = 15, // Bolivia, Brazil (Amazonas), Chile (continental), Dominican Republic, Canada (Nova Scotia), Paraguay
	UTC_n04_00 = 16, // Puerto Rico, Trinidad and Tobago
	UTC_n03_30 = 17, // Canada (Newfoundland)
	UTC_n03_00 = 18, // Argentina; (Summer) Brazil (Brasilia, Rio de Janeiro, Sao Paulo), most of Greenland, Uruguay
	UTC_n02_00 = 19, // Brazil (Fernando de Noronha), South Georgia and the South Sandwich Islands
	UTC_n01_00 = 20, // Portugal (Azores), Cape Verde
	UTC_00_00 = 21,	 // Cote d'Ivoire, Faroe Islands, Ghana, Iceland, Senegal; (Summer) Ireland, Portugal (continental and Madeira)
	UTC_00_00 = 22,	 // Spain (Canary Islands), Morocco, United Kingdom
	UTC_01_00 = 23,	 // Angola, Cameroon, Nigeria, Tunisia; (Summer)Albania, Algeria, Austria, Belgium, Bosnia and Herzegovina,
	UTC_01_00 = 24,	 // Spain (continental), Croatia, Czech Republic, Denmark, Germany, Hungary, Italy, Kinshasa, Kosovo,
	UTC_01_00 = 25,	 // Macedonia, France (metropolitan), the Netherlands, Norway, Poland, Serbia, Slovakia, Slovenia, Sweden, Switzerland
	UTC_02_00 = 26,	 // Libya, Egypt, Malawi, Mozambique, South Africa, Zambia, Zimbabwe, (Summer)Bulgaria, Cyprus, Estonia,
	UTC_02_00 = 27,	 // Finland, Greece, Israel, Jordan, Latvia, Lebanon, Lithuania, Moldova, Palestine, Romania, Syria, Turkey, Ukraine
	UTC_03_00 = 28,	 // Belarus, Djibouti, Eritrea, Ethiopia, Iraq, Kenya, Madagascar, Russia (Kaliningrad Oblast), Saudi Arabia,
	UTC_03_00 = 29,	 // South Sudan, Sudan, Somalia, South Sudan, Tanzania, Uganda, Yemen
	UTC_03_30 = 30,	 // (Summer)Iran
	UTC_04_00 = 31,	 // Armenia, Azerbaijan, Georgia, Mauritius, Oman, Russia (European), Seychelles, United Arab Emirates
	UTC_04_30 = 32,	 // Afghanistan
	UTC_05_00 = 33,	 // Kazakhstan (West), Maldives, Pakistan, Uzbekistan
	UTC_05_30 = 34,	 // India, Sri Lanka
	UTC_05_45 = 35,	 // Nepal
	UTC_06_00 = 36,	 // Kazakhstan (most), Bangladesh, Russia (Ural: Sverdlovsk Oblast, Chelyabinsk Oblast)
	UTC_06_30 = 37,	 // Cocos Islands, Myanmar
	UTC_07_00 = 38,	 // Jakarta, Russia (Novosibirsk Oblast), Thailand, Vietnam
	UTC_08_00 = 39,	 // China, Hong Kong, Russia (Krasnoyarsk Krai), Malaysia, Philippines, Singapore, Taiwan, most of Mongolia, Western Australia
	UTC_09_00 = 40,	 // Korea, East Timor, Russia (Irkutsk Oblast), Japan
	UTC_09_30 = 41,	 // Australia (Northern Territory);(Summer)Australia (South Australia))
	UTC_10_00 = 42,	 // Russia (Zabaykalsky Krai); (Summer)Australia (New South Wales, Queensland, Tasmania, Victoria)
	UTC_10_30 = 43,	 // Lord Howe Island
	UTC_11_00 = 44,	 // New Caledonia, Russia (Primorsky Krai), Solomon Islands
	UTC_11_30 = 45,	 // Norfolk Island
	UTC_12_00 = 46,	 // Fiji, Russia (Kamchatka Krai);(Summer)New Zealand
	UTC_12_45 = 47,	 // (Summer)New Zealand
	UTC_13_00 = 48,	 // Tonga
	UTC_14_00 = 49,	 // Kiribati (Line Islands)
} UTC_timezone_t;


/**
 * @brief UTC offsets in seconds
 */
const int32_t UTC_offsets_s[50] = {
	-43200, -39600, -36000, -34200, -32400, -28800, -28800, -25200, -25200, -21600,
	-21600, -18000, -18000, -18000, -16200, -14400, -14400, -12600, -10800, -7200, -3600,
	0, 0, 3600, 3600, 3600, 7200, 7200, 10800, 10800, 12600, 14400, 16200, 18000,
	19800, 20700, 21600, 23400, 25200, 28800, 32400, 34200, 36000, 37800, 39600, 41400, 43200, 45900, 46800, 50400
};

static const char day_name_str_0[]  = "\0";
static const char day_name_str_1[]  = "Domingo";
static const char day_name_str_2[]  = "Lunes";
static const char day_name_str_3[]  = "Martes";
static const char day_name_str_4[]  = "Miercoles";
static const char day_name_str_5[]  = "Jueves";
static const char day_name_str_6[]  = "Viernes";
static const char day_name_str_7[]  = "Sabado";

/**
 * @brief 
 * 
 */
static const char * const day_name_str_P[]  = { day_name_str_0, day_name_str_1, day_name_str_2, day_name_str_3, day_name_str_4,
day_name_str_5, day_name_str_6, day_name_str_7 };


static const char month_name_str_0[]  = "\0";
static const char month_name_str_1[]  = "Enero";
static const char month_name_str_2[]  = "Febrero";
static const char month_name_str_3[]  = "Marzo";
static const char month_name_str_4[]  = "Abril";
static const char month_name_str_5[]  = "Mayo";
static const char month_name_str_6[]  = "Junio";
static const char month_name_str_7[]  = "Julio";
static const char month_name_str_8[]  = "Agosto";
static const char month_name_str_9[]  = "Septiembre";
static const char month_name_str_10[]  = "Octubre";
static const char month_name_str_11[]  = "Noviembre";
static const char month_name_str_12[]  = "Diciembre";

/**
 * @brief 
 * 
 */
static const char * const month_name_str_P[]  = { month_name_str_0, month_name_str_1, month_name_str_2, month_name_str_3, month_name_str_4,
month_name_str_5, month_name_str_6, month_name_str_7, month_name_str_8, month_name_str_9, month_name_str_10, month_name_str_11,
month_name_str_12 };

#ifdef TIME_EPOCH_MODIFIABLE

    static uint16_t s_epoch_year		= Y2K_EPOCH_YEAR;
    static uint8_t  s_pivot_year		= 0;
    static uint8_t  s_epoch_offset		= 0;
    static uint8_t  s_epoch_weekday     = Y2K_EPOCH_WEEKDAY;
#else
    static const uint16_t s_epoch_year    = Y2K_EPOCH_YEAR;
    static const uint8_t  s_pivot_year    = Y2K_EPOCH_YEAR % 100;
    static const uint8_t  s_epoch_offset  = Y2K_EPOCH_YEAR % 100;
    static const uint8_t  s_epoch_weekday = Y2K_EPOCH_WEEKDAY;
#endif



/**
 * @brief Arreglo de definición de días en mes, se antepone un 0 para que el arreglo comience en el índice 1, correspondiente al mes de Enero
*/
static const uint8_t days_in[] =  {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * Prototipos de funciones
*/

#ifdef TIME_EPOCH_MODIFIABLE
void time_set_pivot_year( uint8_t py );
#endif

uint8_t time_get_pivot_year();

#ifdef TIME_EPOCH_MODIFIABLE
void time_use_fastest_epoch();
#endif


#ifdef TIME_EPOCH_MODIFIABLE
void time_set_epoch_weekday(uint8_t ew );
#endif

uint8_t time_get_epoch_weekday();


#ifdef TIME_EPOCH_MODIFIABLE
void time_set_epoch_offset(uint8_t eo);
#endif

uint8_t time_get_epoch_offset();

#ifdef TIME_EPOCH_MODIFIABLE
void time_set_epoch_year(uint16_t y);
#endif

uint16_t time_get_epoch_year();
uint8_t time_weekday_for(uint16_t dayno);
bool time_isValid(time_t *tiempo);
bool time_isLeapFull(uint16_t year);
uint16_t time_fullYear(time_t *tiempo);
bool time_isLeap(time_t *tiempo);
uint16_t time_daysPer(uint16_t year);
void time_get_real_time(time_t *tiempo, clock_t c);
uint16_t time_dayOfYear(time_t *tiempo);
uint16_t time_getDays(time_t *tiempo);
clock_t time_get_raw_time(time_t *tiempo);
void time_init(time_t *tiempo);
void time_setDay(time_t *tiempo);
#ifdef TIME_EPOCH_MODIFIABLE
void time_use_fastest_epoch();
#endif
void time_GMT_compensation(clock_t *raw_time, UTC_timezone_t compensation_utc);

bool time_parse(const char* s, time_t *tiempo);
#ifdef time_LOG
bool time_print(time_t *tiempo);
#endif

#endif /*TIME_H*/