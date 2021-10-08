/**
 * @file NeoTime.c
 * @brief Librería para manejo de estructuras de datos de tiempo
 * @author Ing. José Roberto Parra Trewartha
*/
#include "NeoTime.h"

/**
 * @brief Función para establecer año pivote. El año de pivote determina el rango de años con respecto al año de la época.
 * Por ejemplo, un año época igual a 2000 y un año pivote de 80 permitirá años en el rango de 1980 a 2079. Valor predeterminado 0 para Y2K EPOCH.
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo a modificar
 * @param py (uint8_t) Dato de año pivote  
 * @return (void)
*/
#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_pivot_year( uint8_t py ) {
	s_pivot_year = py;   
}
#endif

/**
 * @param  Función para obtener año pivote
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo
 * @return (uint8_t) Dato de año pivote 
*/
uint8_t NeoTime_get_pivot_year() {
	return s_pivot_year; 
}

/**
 * @brief Función para establecer día de la semana base, es decir, que día de la semana corresponde al día cero
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @param ew (uint8_t) Día a establecer como base (0-6)
	Valor de retorno:
*/
#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_epoch_weekday(uint8_t ew )  {
	s_epoch_weekday = ew; 
}
#endif

/**
 * @brief Función para obtener día de la semana base, es decir, que día de la semana corresponde al día cero.
 * @param (void)
 * @return (uint8_t) Día base de la semana (0-6)
*/
uint8_t NeoTime_get_epoch_weekday() {
	return s_epoch_weekday;
}

/**
 * @brief Función para establecer offset de época
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @param eo (uint8_t) Offset de época
*/
#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_set_epoch_offset(uint8_t eo) {
	s_epoch_offset = eo;
}
#endif

/**
 * @brief Función para obtener offset de época
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (uint8_t) Offset de época
*/
uint8_t NeoTime_get_epoch_offset() {
	return s_epoch_offset;
}

#ifdef TIME_EPOCH_MODIFIABLE
/**
 * @brief Función para definir el año inicial de una época. Es de notarse que el año pivote se calcula como  epoch_year % 100. Los valores válidos se encuentran
 * en el rango epoch_year..epoch_year+99
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @param y (uint16_t) de época para establecer
 * @return
*/
void NeoTime_set_epoch_year(uint16_t y) {
	s_epoch_year = y;
	NeoTime_set_epoch_offset( s_epoch_year % 100 );
	NeoTime_set_pivot_year( NeoTime_get_epoch_offset() );
}
#endif

/**
 * @brief Función para obtener el año inicial de una época.
 * @param (void)
 * @return (uint16_t) Año inicial de época.
*/
uint16_t NeoTime_get_epoch_year() {
	return s_epoch_year;
}

/**
 * @brief Función para determinar el día de la semana para el número de día especificado
 * @param dayno (uint16_t) Número de día contado a partir del 1° de Enero del año época 
 * @return (uint8_t) Número de día de la semana (1..7) para el día especificado
*/
uint8_t NeoTime_weekday_for(uint16_t dayno) {
	return ((dayno+NeoTime_get_epoch_weekday()-1) % DAYS_PER_WEEK) + 1;
}

/**
 * @brief Función que verifica que todos los parámetros, excepto el día, estén configurados en una fecha / hora coherente.
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (bool) Verdadero si se tiene una fecha y hora válida
*/
bool NeoTime_isValid(time_t *tiempo) {
	return  ((tiempo->year <= 99) && (1 <= tiempo->month) && (tiempo->month <= 12) && ((1 <= tiempo->date) && ((tiempo->date <= days_in[tiempo->month]) || \
            ((tiempo->month == 2) && NeoTime_isLeap(tiempo) && (tiempo->date == 29)))) && (tiempo->hours   <= 23) && (tiempo->minutes <= 59) && (tiempo->seconds <= 59));
}

/**
 * @brief Función para determinar si el año actual (de 4 dígitos) es un año bisiesto.
 * @param year (uint16_t) Año para determinar si es bisiesto
 * @return (bool) Verdadero si el año en cuestión es bisiesto.
*/
bool NeoTime_isLeapFull(uint16_t year) {
	if ( year % 4 )	//Si el año no es divisible entre 4, no es bisiesto
		return false;
	//Si es divisible entre 4, se verifican las condiciones para determinar si un año es bisiesto:
	if( (year % 100) != 0 )	//Si es divisible entre 4 y no es divisible entre 100, entonces es bisiesto
		return true;
	if( year % 400)			//Si es divisible entre 4 y 100, pero no entre 400, entonces no es bisiesto
		return false;
	return true;			//Si es divisible entre 4, 100 y 400, es bisiesto
	
}

/**
 * @brief Función para determinar año real en formato normal
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (uint16_t) Valor de año real
*/
uint16_t NeoTime_fullYear(time_t *tiempo) {
	uint16_t y = tiempo->year;
	uint16_t epoch_year = NeoTime_get_epoch_year();

    if (y < NeoTime_get_pivot_year())
      y += 100 * (epoch_year/100 + 1);
    else
      y += 100 * (epoch_year/100);
    return y;
}

/**
 * @brief Función para determinar si el año actual (de 2 dígitos) es un año bisiesto.
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (bool) Verdadero si el año en cuestión es bisiesto.
*/
bool NeoTime_isLeap(time_t *tiempo) {
	return NeoTime_isLeapFull(NeoTime_fullYear(tiempo));
}

/**
 * @brief Función para calcular cuántos días hay en el año especificado.
 * @param year (uint16_t) Año (4 dígitos).
 * @return (uint16_t) Número de días en ese año
*/
uint16_t NeoTime_daysPer(uint16_t year) {
	return (NeoTime_isLeapFull(year))? 366:365;
}

/**
 * @brief Función para cálculo de variables de tiempo, a partir de variable global entera de tiempo (clock_t)
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @param c (clock_t) Valor de tiempo UTC
 * @return (void)
 * 
*/
void NeoTime_get_real_time(time_t *tiempo, clock_t c) {
	uint16_t dayno = c / SECONDS_PER_DAY;
	c -= dayno * (uint32_t)SECONDS_PER_DAY;
	tiempo->day = NeoTime_weekday_for(dayno);		//Cálculo de número de día

	uint16_t y = NeoTime_get_epoch_year();			//Obtención de año época (año base o 0)
	for (;;) {
		uint16_t days = NeoTime_daysPer( y );
		if (dayno < days)
			break;
		dayno -= days;
		y++;
	}
  	bool leap_year = NeoTime_isLeapFull(y);			//Bandera de año bisiesto
	y -= NeoTime_get_epoch_year();
	y += NeoTime_get_epoch_offset();
	while (y > 100)
		y -= 100;
	tiempo->year = y;						//Asignación de año con respecto al base

	uint8_t month = 1;
	for (;;)
	{
		uint8_t days = days_in[month];
		if (leap_year && (month == 2))	//Si el año es bisiesto y el mes es Febrero... 
			days++;						//...agregar un día
		if (dayno < days)
			break;
		dayno -= days;
		month++;
	}
	tiempo->month = month;				//Asignación de mes
	tiempo->date = dayno + 1;			//Asignación de día

	tiempo->hours = (c / SECONDS_PER_HOUR);	//Asignación de hora
	uint8_t hours = tiempo->hours;
	uint16_t c_ms;
	if (hours < 18) 
		c_ms = (uint16_t) c - (hours * (uint16_t) SECONDS_PER_HOUR);
	else
		c_ms = c - (hours * (uint32_t) SECONDS_PER_HOUR);
	tiempo->minutes = c_ms / SECONDS_PER_MINUTE;					//Asignación de minutos
	tiempo->seconds = c_ms - (tiempo->minutes * SECONDS_PER_MINUTE);		//Asignación de segundos
}

/**
 * @brief Función para obtener cantidad de días que han transcurrido desde el inicio del año actual. El 1° de Enero es el día 0, 
 * por lo que si han transcurrido 'n' días, el resultado será 'n-1'
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (uint16_t) Número de día del año
*/
uint16_t NeoTime_dayOfYear(time_t *tiempo) {
	uint16_t dayno = tiempo->date - 1;		
	bool leap_year = NeoTime_isLeap(tiempo);	//Determina si es año bisiesto
    uint8_t mes = tiempo->month;
	for (uint8_t m = 1; m != mes; m++)
	{
		dayno += days_in[m];			//Incrementa número de días
		if (leap_year && (m == 2))		//Si el año es bisiesto y el mes es Febrero...
			dayno++;					//...agregar un día
	}

	return dayno;
}

/**
 * @brief Función para obtener cantidad de días desde el año época. Si han transcurrido 'n' días, el resultado será 'n-1'
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (uint16_t) Cantidad de días transcurridos desde año época
*/
uint16_t NeoTime_getDays(time_t *tiempo) {
	uint16_t day_count = NeoTime_dayOfYear(tiempo);

	uint16_t y = NeoTime_fullYear(tiempo);
	while (y-- != NeoTime_get_epoch_year())
		day_count += NeoTime_daysPer(y);
	return day_count;
}

/**
 * @brief Función para cálculo de variable global entera de tiempo, a partir de estructura de datos de variables de tiempo
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (clock_t) Tiempo UTC
*/
clock_t NeoTime_get_raw_time(time_t *tiempo) {
	clock_t c = NeoTime_getDays(tiempo) * SECONDS_PER_DAY;
	uint8_t hours = tiempo->hours;
	if(hours < 18)
		c += hours * (uint16_t) SECONDS_PER_HOUR;
	else
		c += hours * (uint32_t) SECONDS_PER_HOUR;
	c += tiempo->minutes* (uint16_t) SECONDS_PER_MINUTE;
	c += tiempo->seconds;

	return c;
}

/**
 * @brief Función de inicialización de estructura de datos de tiempo
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (void)
*/
void NeoTime_init(time_t *tiempo) {
	tiempo->seconds = 0;
	tiempo->hours = 0;
	tiempo->minutes = 0;
	tiempo->date = 1;
	tiempo->month = 1;
	tiempo->year = NeoTime_get_epoch_year() % 100;
	tiempo->day = NeoTime_get_epoch_weekday();

}

/**
 * @brief Función para configurar el miembro del día a partir del valor actual. Esta es una operación relativamente costosa, por lo que
 * el día de la semana solo se calcula cuando se solicita.
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (void)
*/
void NeoTime_setDay(time_t *tiempo) {
	tiempo->day = NeoTime_weekday_for(NeoTime_getDays(tiempo));
}

#ifdef TIME_EPOCH_MODIFIABLE
void NeoTime_use_fastest_epoch() {
	// Averigua cuándo fuimos compilados y usamos el año para una época muy rápida. Formato "MMM DD AAAA"
	const char* compile_date = (const char *)(__DATE__);
	uint16_t    compile_year = 0;
	for (uint8_t i = 7; i != 11; i++)	//Posiciones que ocupa el año en la macro __DATE__
		compile_year = compile_year*10 + compile_date[i] - '0';

	// Establece temporalmente una época Y2K para que podamos determinar el día del 1 de enero de este año.
	NeoTime_set_epoch_year      ( Y2K_EPOCH_YEAR );
	NeoTime_set_epoch_weekday   ( Y2K_EPOCH_WEEKDAY );

	time_t this_year;
	this_year.year = compile_year % 100;
	NeoTime_setDay(&this_year);
	uint8_t compile_weekday = this_year.day;

	NeoTime_set_epoch_year   ( compile_year );
	NeoTime_set_epoch_weekday( compile_weekday );
	NeoTime_set_pivot_year   ( this_year.year );
}
#endif

/**
 * 
 * @param raw_time
 * @param compensation_gmt
 */
void NeoTime_GMT_compensation(clock_t *raw_time, int8_t compensation_gmt){
    int32_t compensation_seconds = (int32_t)compensation_gmt * SECONDS_PER_HOUR;
    #ifdef NEOTIME_LOG
    printf("GMT %d. Compensation in seconds: %ld\r\n",compensation_gmt,compensation_seconds);
    #endif
    *raw_time += compensation_seconds;
    
    #ifdef NEOTIME_LOG
    printf("New NTP time: %lu\r\n",*raw_time);
    #endif
}


/**
 * @brief Función para separar los parámetros de tiempo a partir de una cadena de caracteres
 * @param s (const char*) Apuntador a cadena de caracteres con formato "YYYY-MM-DD HH:MM:SS".
 * @param tiempo (time_t*) Apuntador a estructura de datos de tiempo.
 * @return (bool) Verdadero si hubo éxito
*/
bool NeoTime_parse(const char* s, time_t *tiempo) {
	char buf[32];
	strcpy(buf, s);
	char* sp = &buf[0];
	uint16_t value = strtoul(sp, &sp, 10);

	if (*sp != '-') return false;
	tiempo->year = value % 100;
	if (NeoTime_fullYear(tiempo) != value) return false;

	value = strtoul(sp + 1, &sp, 10);
	if (*sp != '-') return false;
	tiempo->month = value;

	value = strtoul(sp + 1, &sp, 10);
	if (*sp != ' ') return false;
	tiempo->date = value;

	value = strtoul(sp + 1, &sp, 10);
	if (*sp != ':') return false;
	tiempo->hours = value;

	value = strtoul(sp + 1, &sp, 10);
	if (*sp != ':') return false;
	tiempo->minutes = value;

	value = strtoul(sp + 1, &sp, 10);
	if (*sp != 0) return false;
	tiempo->seconds = value;

	return (NeoTime_isValid(tiempo));
}

#ifdef NEOTIME_LOG
/**
 * 
 * @param tiempo
 * @return 
 */
bool NeoTime_print(time_t *tiempo){
    uint16_t year = tiempo->year;
    uint8_t  month = tiempo->month;
    uint8_t  date = tiempo->date;
    uint8_t  day = tiempo->day;
    uint8_t  hours = tiempo->hours;
    uint8_t  minutes = tiempo->minutes;
    uint8_t  seconds = tiempo->seconds;
    printf("%s %u-%02u-%02u %02u:%02u:%02u\r\n",day_name_str_P[day],date,month,year,hours,minutes,seconds);
    bool valid = NeoTime_isValid(tiempo);
    printf("%s\r\n",valid? "Valid time":"Invalid time");
    return valid;
}
#endif                       