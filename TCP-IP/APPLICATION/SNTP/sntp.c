/**
 * @file sntp.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-01-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "sntp.h"

/**
 * @brief
 *
 * @param socket_number
 * @param ntp_server
 * @param time_zone
 */
void SNTP_init(socket_t socket_number, const char *ntp_server, UTC_timezone_t timezone)
{
    sntp_client.socket = socket_number;
    sntp_client.ntp_server = ntp_server;
    sntp_client.timezone = timezone;
    sntp_client.format.version = NTP_CURRENT_VERSION;
    sntp_client.format.leap = NTP_LI_NO_WARNING;
    sntp_client.format.mode = NTP_MODE_CLIENT;
    sntp_client.format.stratum = NTP_STRATUM_UNSPECIFIED;
    sntp_client.format.poll = 0;
    sntp_client.format.precision = 0;
    sntp_client.format.rootdelay = 0;
    sntp_client.format.rootdisp = 0;
    sntp_client.format.refid = 0;
    memset(&sntp_client.format.reftime, 0, sizeof(timestamp_t));
    memset(&sntp_client.format.org, 0, sizeof(timestamp_t));
    memset(&sntp_client.format.rec, 0, sizeof(timestamp_t));
    memset(&sntp_client.format.xmt, 0, sizeof(timestamp_t));

    // NTP buffer initialization
    uint8_t sntp_flag = ((NTP_LI_NO_WARNING << NTP_LEAP_INDICATOR_BITSHIFT) | (NTP_CURRENT_VERSION << NTP_LEAP_VERSION_NUMBER_BITSHIFT) | (NTP_MODE_CLIENT << NTP_MODE_BITSHIFT));
    memset(sntp_client.NTP_buffer, 0, NTP_BUFFER_SIZE);
    sntp_client.NTP_buffer[0] = sntp_flag;

    
}

/**
 * @brief
 *
 * @param time_zone
 * @return clock_t
 */
clock_t SNTP_getUTC(UTC_timezone_t time_zone)
{
    
}

/**
 * @brief
 *
 * @param time
 * @return socket_error_t
 */
socket_error_t SNTP_run(time_t *time)
{
    socket_error_t retVal = SOCK_ERROR;
    uint8_t socket_status = getSn_SR(sntp_client.socket);
    if(socket_status == SOCK_UDP){
        // Socket is opened in UDP mode already

    } else if(socket_status == SOCK_CLOSED){
        // Creation of new UDP socket
        retVal = socket(sntp_client.socket,SOCK_UDP,NTP_PORT,0);
    }
    
}