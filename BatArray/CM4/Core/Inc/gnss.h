/*
 * gnss.h
 *
 *  Created on: Oct 28, 2022
 *      Author: Phil
 *
 */

#ifndef SRC_GPS_H_
#define SRC_GPS_H_

#include "configuration.h"
#include "u_ubx_protocol.h"
#include "time.h"
#include "stdbool.h"
#include "usart.h"

// Macros
#define GNSS_CONFIG_BUFFER_SIZE 600
#define CONFIGURATION_ARRAY_SIZE 164
#define MAX_POSSIBLE_VELOCITY 10000     // 10000 mm/s = 10 m/s
#define UBX_NAV_PVT_MESSAGE_CLASS 0x01
#define UBX_NAV_PVT_MESSAGE_ID 0x07
#define UBX_NAV_PVT_MESSAGE_LENGTH 100
#define UBX_CFG_VALSET_CLASS 0x06
#define UBX_CFG_VALSET_ID 0x8A
#define UBX_MESSAGE_SIZE (92 + U_UBX_PROTOCOL_OVERHEAD_LENGTH_BYTES)
#define UBX_BUFFER_SIZE (10 * UBX_MESSAGE_SIZE)
#define INITIAL_STAGES_BUFFER_SIZE 500
#define FRAME_SYNC_RX_SIZE 200
#define UBX_NAV_PVT_PAYLOAD_LENGTH 92
#define UBX_ACK_MESSAGE_LENGTH 10
#define MAX_ACCEPTABLE_SACC 250 // need to confirm with Jim what this should be
#define MAX_ACCEPTABLE_PDOP 1000 // (units = 0.01) greater than 10 means fair fix accuracy
#define MAX_EMPTY_QUEUE_WAIT 50 // wait for max 50ms
#define MAX_EMPTY_CYCLES 5*60*10 // no data for 10 mins
#define MAX_FRAME_SYNC_ATTEMPTS 3
#define MAX_CONFIG_STEP_ATTEMPTS 3
#define GNSS_DEFAULT_BAUD_RATE 9600
#define MAX_THREADX_WAIT_TICKS_FOR_CONFIG (TX_TIMER_TICKS_PER_SECOND + (TX_TIMER_TICKS_PER_SECOND / 4))
#define ONE_SECOND 1000
#define MILLISECONDS_PER_MINUTE 60000
#define MM_PER_METER 1000.0
#define MIN_SATELLITES_TO_PASS_TEST 4
#define LOWER_4_BITS_MASK 0xF
#define LAT_LON_CONVERSION_FACTOR 10000000 // format as 1E-7
#define GNSS_TIMER_INSTANCE TIM16
// UBX message definitions
#define RESOLVED_TIME_BITS ((1 << 0) | (1 << 2)) // Only resolve to the time of day
#define UBX_NAV_PVT_YEAR_INDEX 4
#define UBX_NAV_PVT_MONTH_INDEX 6
#define UBX_NAV_PVT_DAY_INDEX 7
#define UBX_NAV_PVT_HOUR_INDEX 8
#define UBX_NAV_PVT_MINUTE_INDEX 9
#define UBX_NAV_PVT_SECONDS_INDEX 10
#define UBX_NAV_PVT_VALID_FLAGS_INDEX 11
#define UBX_NAV_PVT_TACC_INDEX 12
#define UBX_NAV_PVT_VALID_FLAGS2_INDEX 22
#define UBX_NAV_PVT_NUMSV_INDEX 23
#define UBX_NAV_PVT_LON_INDEX 24
#define UBX_NAV_PVT_LAT_INDEX 28
#define UBX_NAV_PVT_HACC_INDEX 40
#define UBX_NAV_PVT_VACC_INDEX 44
#define UBX_NAV_PVT_V_NORTH_INDEX 48
#define UBX_NAV_PVT_V_EAST_INDEX 52
#define UBX_NAV_PVT_V_DOWN_INDEX 56
#define UBX_NAV_PVT_SACC_INDEX 68
#define UBX_NAV_PVT_PDOP_INDEX 76
#define UBX_ACK_ACK_CLSID_INDEX 0
#define UBX_ACK_ACK_MSGID_INDEX 1
#define HIGH_PERFORMANCE_QUERY_SIZE 28
#define HIGH_PERFORMANCE_RESPONSE_SIZE 36
#define ENABLE_HIGH_PERFORMANCE_SIZE 60

typedef enum
{
  AS_LITTLE_ENDIAN = 0,
  AS_BIG_ENDIAN = 1
} endian_t;

// GNSS struct definition -- packed for good organization, not memory efficiency
typedef struct GNSS
{
  UART_HandleTypeDef *gnss_uart_handle;
  time_t sample_window_start_time;
  // The start time for the sampling window
  time_t sample_window_stop_time;

  uint32_t messages_processed;
  uint32_t number_cycles_without_data;
  uint32_t total_samples;
  bool is_time_resolved;
} GNSS;

/* Function declarations */
void gnss_init ( void );
bool gnss_config ( void );
bool gnss_sync ( void );
bool gnss_get_time ( struct tm *return_time );

#endif /* SRC_GPS_H_ */
