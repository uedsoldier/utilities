/**
 * @file sntp.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-01-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SNTP_H
#define SNTP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "../../TRANSPORT/socket.h"
#include "../../NETWORK/IPV4/IPv4.h"
#include "../../TIME/time.h"

	/**
	 * @brief Define it for Debug & Monitor DNS processing.
	 * @note If defined, it dependens on <stdio.h>
	 * */
	// #define _SNTP_DEBUG_

/**
 * @brief
 *
 */
#define NTP_PACKET_SIZE 48

/**
 * @brief
 *
 */
#define NTP_PORT 123

/**
 * @brief
 *
 */
#define NTP_CURRENT_VERSION 4

	/**
	 * @brief This is a two-bit code warning of an impending leap second to be inserted
	 * or deleted in the last minute of the current day, with bit 0 and bit 1, respectively, coded as follows:
	 * 00 -> No warning
	 * 01 -> Last minute of the day has 61 s
	 * 10 -> Last minute of the day has 59 s
	 * 11 -> Alarm condition (clock not synchronized)
	 */
	typedef enum NTP_leap_indicator_t
	{
		NTP_LI_NO_WARNING = 0b00,
		NTP_LI_LASTMINUTE_61 = 0b01,
		NTP_LI_LASTMINUTE_59 = 0b10,
		NTP_LI_ALARM_CONDITION = 0b11
	} NTP_leap_indicator_t;

	/**
	 * @brief This is a three-bit integer indicating the mode, with values defined as follows:
	 * 0 -> Reserved
	 * 1 -> Symmetric active
	 * 2 -> Symmetric passive
	 * 3 -> Client
	 * 4 -> Server
	 * 5 -> Broadcast
	 * 6 -> NTP control message
	 * 7 -> Reserved for private use
	 */
	typedef enum NTP_mode_t
	{
		NTP_MODE_RESERVED = 0,
		NTP_MODE_SYMMETRIC_ACTIVE = 1,
		NTP_MODE_SYMMETRIC_PASSIVE = 2,
		NTP_MODE_CLIENT = 3,
		NTP_MODE_SERVER = 4,
		NTP_MODE_BROADCAST = 5,
		NTP_MODE_CONTROL_MESSAGE = 6,
		NTP_MODE_RESERVED_PRIVATE = 7
	} NTP_mode_t;

	/**
	 * @brief This is an eight-bit integer indicating the stratum level of the system clock, with values defined as follows:
	 * 0 -> Unspecified
	 * 1 -> Reference clock (e.g., radio clock)
	 * 2-15 -> Secondary server (via NTP)
	 * 16-255 -> Unreachable
	 * For convenience, the value 0 in received packets is mapped to 16 as a peer variable, and a peer variable of 16-255 is
	 * mapped to 0 in transmitted packets. This allows reference clocks, which normally appear at stratum 0, to be conveniently
	 * mitigated using the same algorithms used for external sources.
	 */
	typedef enum NTP_stratum_t
	{
		NTP_STRATUM_UNSPECIFIED = 0,
		NTP_STRATUM_REFERENCE_CLOCK = 1,
		NTP_STRATUM_SECONDARY_SERVER = 2,
		NTP_STRATUM_UNREACHABLE = 16
	} NTP_stratum_t;

	/**
	 * @brief Timestamp data structure. 64-bit
	 *
	 */
	typedef struct timestamp_t
	{
		union
		{
			uint8_t timestamp_bytes[8];
			uint16_t timestamp_half_words[4]
		};

	} timestamp_t;

	/**
	 * @brief
	 *
	 */
#define SNTP_TIMESTAMP_SIZE sizeof(timestamp_t)

	typedef struct NTP_format_t
	{
		IPV4_address_t dstaddr;	   // Destination (local) address
		int8_t version;			   // Version number
		NTP_leap_indicator_t leap; // Leap indicator
		NTP_mode_t mode;		   // Mode
		NTP_stratum_t stratum;	   // Stratum
		uint8_t poll;			   // Poll exponent: This is an eight-bit unsigned integer indicating the maximum interval between successive messages, in seconds to the nearest power of two. In the reference implementation, the values can range from 3 (8 s) through 17 (36 h).
		int8_t precision;		   // Precision: This is an eight-bit signed integer indicating the precision of the system clock, in seconds to the nearest power of two. For instance, a value of -18 corresponds to a precision of about 1 ]s. The precision is normally measured by the daemon at startup and is defined as the minimum of several iterations of the time to read the system clock, which normally is done by a kernel system call. When the clock is not interpolated within the tick, the precision is set at the tick interval.
		uint32_t rootdelay;		   // Root delay: This is a 32-bit, unsigned, fixed-point number indicating the total round-trip delay to the reference clock, in seconds with fraction point between bits 15 and 16. In contrast to the calculated peer round-trip delay, which can take both positive and negative values, this value is always positive.
		uint32_t rootdisp;		   // Root dispersion: This is a 32-bit, unsigned, fixed-point number indicating the maximum error relative to the reference clock, in seconds with fraction point between bits 15 and 16.
		uint32_t refid;			   // Reference ID: This is a 32-bit code identifying the particular reference clock.
		timestamp_t reftime;	   // Reference time: This is the local time at which the system clock was last set or corrected, in 64-bit NTP timestamp format.
		timestamp_t org;		   // Origin timestamp: This is the local time at which the request departed the client for the server, in 64-bit NTP timestamp format.
		timestamp_t rec;		   // Receive timestamp: This is the local time at which the request arrived at the server, in 64-bit NTP timestamp format
		timestamp_t xmt;		   // Transmit timestamp: This is the local time at which the reply departed the server for the client, in 64-bit NTP timestamp format
								   // uint32_t key_identifier; // When the NTP authentication scheme is in use, this contains the 32-bit key identifier followed by the 128-bit MD5 message digest
	} NTP_format_t;

	/**
	 * @brief
	 *
	 */
#define SNTP_MAX_BUFFER_SIZE sizeof(NTP_format_t)

	typedef struct sntp_client_t
	{
		socket_t socket;
		uint8_t *ntp_server;
		time_t now;
		UTC_timezone_t timezone;
		uint8_t NTP_retryCount;
		uint8_t NTP_buffer[NTP_PACKET_SIZE];
		NTP_format_t format;
	} sntp_client_t;

	static sntp_client_t sntp_client;

#define NTP_LEAP_INDICATOR_BITSHIFT 6
#define NTP_LEAP_VERSION_NUMBER_BITSHIFT 3
#define NTP_MODE_BITSHIFT 0

/**
 * @brief 
 * 
 */
#define NTP_BUFFER_SIZE (sizeof(sntp_client.NTP_buffer))

#pragma region Function prototypes
void SNTP_init(socket_t socket_number, const char *ntp_server, UTC_timezone_t timezone);
clock_t SNTP_getUTC(UTC_timezone_t time_zone);
socket_error_t SNTP_run(time_t *time);
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif /* SNTP_H */