/**
 * @file NeoTime.h
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

//
#define NEOTIME_LOG

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
	SUNDAY 		= 1,	// Domingo	
	MONDAY 		= 2,	// Lunes
	TUESDAY 	= 3,	// Martes	
	WEDNESDAY 	= 4,	// Miércoles
	THURSDAY 	= 5,	// Jueves
	FRIDAY 		= 6,	// Viernes
	SATURDAY 	= 7		// Sábado
} weekday_t;

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

static const char day_name_str_0[]  = "\0";
static const char day_name_str_1[]  = "Domingo";
static const char day_name_str_2[]  = "Lunes";
static const char day_name_str_3[]  = "Martes";
static const char day_name_str_4[]  = "Miercoles";
static const char day_name_str_5[]  = "Jueves";
static const char day_name_str_6[]  = "Viernes";
static const char day_name_str_7[]  = "Sabado";
static const char * const day_name_str_P[]  = { day_name_str_0, day_name_str_1, day_name_str_2, day_name_str_3, day_name_str_4, day_name_str_5, day_name_str_6, day_name_str_7 };

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
void NeoTime_set_pivot_year( uint8_t py );
#endif

uint8_t NeoTime_get_pivot_year();

#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_use_fastest_epoch();
#endif


#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_epoch_weekday(uint8_t ew );
#endif

uint8_t NeoTime_get_epoch_weekday();


#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_epoch_offset(uint8_t eo);
#endif

uint8_t NeoTime_get_epoch_offset();

#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_epoch_year(uint16_t y);
#endif

uint16_t NeoTime_get_epoch_year();
uint8_t NeoTime_weekday_for(uint16_t dayno);
bool NeoTime_isValid(time_t *tiempo);
bool NeoTime_isLeapFull(uint16_t year);
uint16_t NeoTime_fullYear(time_t *tiempo);
bool NeoTime_isLeap(time_t *tiempo);
uint16_t NeoTime_daysPer(uint16_t year);
void NeoTime_get_real_time(time_t *tiempo, clock_t c);
uint16_t NeoTime_dayOfYear(time_t *tiempo);
uint16_t NeoTime_getDays(time_t *tiempo);
clock_t NeoTime_get_raw_time(time_t *tiempo);
void NeoTime_init(time_t *tiempo);
void NeoTime_setDay(time_t *tiempo);
#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_use_fastest_epoch();
#endif
void NeoTime_GMT_compensation(clock_t *raw_time, int8_t compensation_gmt);


bool NeoTime_parse(const char* s, time_t *tiempo);
#ifdef NEOTIME_LOG
bool NeoTime_print(time_t *tiempo);
#endif

#endif /*TIME_H*/