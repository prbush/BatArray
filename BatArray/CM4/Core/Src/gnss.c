/*
 * gnss.c
 *
 *  Created on: Oct 28, 2022
 *      Author: Phil
 *
 */

#include "gnss.h"
#include "usart.h"
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "time.h"
#include "u_ubx_protocol.h"
#include "u_error_common.h"
#include "gpio.h"

static bool gnss_reset_uart ( void );

// Static helper functions
static bool send_config ( uint8_t *config_array, size_t message_size, uint8_t response_class,
                          uint8_t response_id );
static void process_frame_sync_messages ( uint8_t *process_buf );
static uint16_t get_two_bytes ( uint8_t *byte_array, uint32_t start_index, endian_t endianess );

static GNSS self;

static bool rx_complete = false;
static uint16_t received_bytes = 0;

static uint8_t config[CONFIGURATION_ARRAY_SIZE] =
  { 0xB5, 0x62, 0x06, 0x8A, 0x9C, 0x00, 0x01, 0x01, 0x00, 0x00, 0xBA, 0x00, 0x91, 0x20, 0x00, 0xBE,
    0x00, 0x91, 0x20, 0x00, 0xBB, 0x00, 0x91, 0x20, 0x00, 0xC9, 0x00, 0x91, 0x20, 0x00, 0xCD, 0x00,
    0x91, 0x20, 0x00, 0xCA, 0x00, 0x91, 0x20, 0x00, 0xBF, 0x00, 0x91, 0x20, 0x00, 0xC3, 0x00, 0x91,
    0x20, 0x00, 0xC0, 0x00, 0x91, 0x20, 0x00, 0xC4, 0x00, 0x91, 0x20, 0x00, 0xC8, 0x00, 0x91, 0x20,
    0x00, 0xC5, 0x00, 0x91, 0x20, 0x00, 0xAB, 0x00, 0x91, 0x20, 0x00, 0xAF, 0x00, 0x91, 0x20, 0x00,
    0xAC, 0x00, 0x91, 0x20, 0x00, 0xB0, 0x00, 0x91, 0x20, 0x00, 0xB4, 0x00, 0x91, 0x20, 0x00, 0xB1,
    0x00, 0x91, 0x20, 0x00, 0x07, 0x00, 0x91, 0x20, 0x01, 0x21, 0x00, 0x11, 0x20, 0x08, 0x04, 0x00,
    0x93, 0x10, 0x00, 0x01, 0x00, 0x21, 0x30, 0xC8, 0x00, 0x02, 0x00, 0x21, 0x30, 0x01, 0x00, 0x07,
    0x00, 0x92, 0x20, 0x00, 0x06, 0x00, 0x92, 0x20, 0x00, 0x0A, 0x00, 0x92, 0x20, 0x00, 0x0D, 0x00,
    0x31, 0x10, 0x00, 0x0F, 0x00, 0x31, 0x10, 0x01, 0x18, 0x00, 0x31, 0x10, 0x01, 0xA4, 0x00, 0x11,
    0x20, 0x14, 0x18, 0x5C };

/**
 * Initialize the GNSS struct
 *
 * @return void
 */
void gnss_init ( void )
{
  self.gnss_uart_handle = &huart4;
  self.sample_window_start_time = 0;
  self.sample_window_stop_time = 0;
  self.messages_processed = 0;
  self.number_cycles_without_data = 0;
  self.total_samples = 0;
  self.is_time_resolved = false;

  HAL_GPIO_WritePin (GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
  HAL_Delay (10);
}

/**
 * Configure the MAX-M10S chip by sending a series of UBX_CFG_VALSET messages
 *
 * @return GNSS_SUCCESS or
 *                 GNSS_CONFIG_ERROR if response was not received
 */
bool gnss_config ( void )
{
  uint32_t start_time = HAL_GetTick (), timeout = 30000;

  bool config_success = false;

  while ( (HAL_GetTick () - start_time) < timeout )
  {

    // Send over the configuration settings for RAM
    config_success = send_config (&(config[0]), CONFIGURATION_ARRAY_SIZE, UBX_CFG_VALSET_CLASS,
    UBX_CFG_VALSET_ID);

    if ( !config_success )
    {
      HAL_Delay (10);
      continue;
    }

    return true;
  }

  return false;
}

/**
 *
 *
 * @return gnss_error_code_t
 */
bool gnss_sync ( void )
{
  uint8_t msg_buf[INITIAL_STAGES_BUFFER_SIZE];
  uint32_t start_time = HAL_GetTick (), timeout = 30000, uart_timeout = 1250, uart_start_time = 0,
      elapsed_time = 0;
  HAL_StatusTypeDef hal_ret;

  // Grabbing and processing 5 samples takes ~ 1 second, so we'll keep trying until we hit
  // the gnss_max_acquisition_wait_time
  while ( (HAL_GetTick () - start_time) < timeout )
  {
    // Grab 5 UBX_NAV_PVT messages
    hal_ret = HAL_UART_Receive_DMA (self.gnss_uart_handle, &(msg_buf[0]),
    INITIAL_STAGES_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);
    // Make sure we receive the response within the right amount of time
    if ( hal_ret != HAL_OK )
    {
      HAL_Delay (1);
      gnss_reset_uart ();
      return false;
    }

    uart_start_time = HAL_GetTick ();
    while ( (elapsed_time < uart_timeout) && !rx_complete )
    {
      elapsed_time = HAL_GetTick () - uart_start_time;
    }

    if ( !rx_complete )
    {
      gnss_reset_uart ();
      return false;
    }

    rx_complete = false;

    process_frame_sync_messages (msg_buf);
    // this both ensures we have frame sync'd with the GNSS sensor and are safe
    // to kick off circular DMA receive
    if ( self.messages_processed == 5 && self.number_cycles_without_data == 0
         && self.total_samples == 5 )
    {
      gnss_reset_uart ();
      return true;
    }
    else
    {
      // Short delay to help get the frame sync'd
      HAL_Delay (1);
    }
  }

  gnss_reset_uart ();
  return false;

}

/**
 * Reinitialize the GNSS UART port. Required when switching between Tx and Rx.
 *
 * @param self - GNSS struct
 * @param baud_rate - baud rate to set port to
 */
static bool gnss_reset_uart ( void )
{

  HAL_UART_DMAStop (self.gnss_uart_handle);

  HAL_UART_DeInit (self.gnss_uart_handle);

  return uart4_init ();
}

/**
 * Set the RTC clock.
 *
 * @param GNSS - GNSS struct
 * @param msg_payload - UBX_NAV_PVT message payload containing
 *        time information.
 *
 * @return GNSS_SUCCESS or
 *                 GNSS_RTC_ERROR - if setting RTC returned an error
 */
bool gnss_get_time ( time_t *return_time )
{
  struct tm time;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t time_flags;
  uint32_t buf_length = 500;
  uint8_t receive_buf[buf_length];
  uint32_t uart_timeout = 1250, start_time = 0, elapsed_time = 0;
  uint8_t payload[UBX_NAV_PVT_PAYLOAD_LENGTH];
  const char *buf_start = (const char*) &(receive_buf[0]);
  const char *buf_end = buf_start;
  int32_t message_class = 0;
  int32_t message_id = 0;
  int32_t num_payload_bytes = 0;
  bool is_ubx_nav_pvt_msg;

  if ( HAL_UART_Receive_DMA (self.gnss_uart_handle, &(receive_buf[0]), buf_length) != HAL_OK )
  {
    gnss_reset_uart ();
    return false;
  }
  __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);

  start_time = HAL_GetTick ();
  while ( (elapsed_time < uart_timeout) && !rx_complete )
  {
    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( !rx_complete )
  {
//    gnss_reset_uart ();
    return false;
  }

  rx_complete = false;

  for ( num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                                (char*) payload, sizeof(payload), &buf_end);
      num_payload_bytes > 0;
      num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                              (char*) payload, sizeof(payload), &buf_end) )
  {

    is_ubx_nav_pvt_msg = (num_payload_bytes == UBX_NAV_PVT_PAYLOAD_LENGTH)
                         && (message_class == UBX_NAV_PVT_MESSAGE_CLASS)
                         && (message_id == UBX_NAV_PVT_MESSAGE_ID);

    if ( !is_ubx_nav_pvt_msg )
    {
      self.number_cycles_without_data++;
      buf_length -= buf_end - buf_start;
      buf_start = buf_end;
      continue;
    }

    year = (int16_t) get_two_bytes (&(payload[0]), UBX_NAV_PVT_YEAR_INDEX, AS_LITTLE_ENDIAN);
    month = payload[UBX_NAV_PVT_MONTH_INDEX];
    day = payload[UBX_NAV_PVT_DAY_INDEX];
    hour = payload[UBX_NAV_PVT_HOUR_INDEX];
    min = payload[UBX_NAV_PVT_MINUTE_INDEX];
    sec = payload[UBX_NAV_PVT_SECONDS_INDEX];
    time_flags = payload[UBX_NAV_PVT_VALID_FLAGS_INDEX];

    time_flags &= LOWER_4_BITS_MASK;

    if ( !(time_flags & RESOLVED_TIME_BITS) )
    {
//      gnss_reset_uart ();
      return false;
    }

    time.tm_year = year;
    time.tm_mon = month;
    time.tm_mday = day;
    time.tm_hour = hour;
    time.tm_min = min;
    time.tm_sec = sec;

    *return_time = mktime (&time);

    return true;
  }

  return false;
}

/**
 * Send a configuration to the GNSS chip. Will retry up to 10 times before
 * returning failure.
 *
 * @param self- GNSS struct
 * @param config_array - byte array containing a UBX_CFG_VALSET msg with up to
 *                64 keys
 */
static bool send_config ( uint8_t *config_array, size_t message_size, uint8_t response_class,
                          uint8_t response_id )
{
  int frame_sync_attempts = 0;
  char payload[UBX_NAV_PVT_PAYLOAD_LENGTH];
  size_t buf_length = 600;
  char config_response_buf[buf_length];
  const char *buf_start = (const char*) &(config_response_buf[0]);
  const char *buf_end = buf_start;
  HAL_StatusTypeDef hal_ret;
  int32_t message_class = 0;
  int32_t message_id = 0;
  int32_t num_payload_bytes = 0;
  uint32_t uart_timeout = 1500;
  uint8_t response_msg_class;
  uint8_t response_msg_id;
  uint16_t max_frame_sync_attempts = 5;
  uint32_t start_time = 0, elapsed_time = 0;
  uint32_t delay;

  // Start by waiting until the UART is idle
  while ( frame_sync_attempts < max_frame_sync_attempts )
  {
    delay = rand () % 19;
    HAL_Delay (delay);

    hal_ret = HAL_UARTEx_ReceiveToIdle_DMA (self.gnss_uart_handle,
                                            (uint8_t*) &(config_response_buf[0]),
                                            FRAME_SYNC_RX_SIZE);

    __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);

    if ( (hal_ret != HAL_OK) )
    {
      frame_sync_attempts++;
      gnss_reset_uart ();
      continue;
    }

    start_time = HAL_GetTick ();
    while ( (elapsed_time < uart_timeout) && !rx_complete )
    {
      elapsed_time = HAL_GetTick () - start_time;
    }

    if ( !rx_complete )
    {
      //    gnss_reset_uart ();
      return false;
    }

    if ( received_bytes != FRAME_SYNC_RX_SIZE )
    {
      received_bytes = 0;
      break;
    }
  }

  received_bytes = 0;

  if ( frame_sync_attempts == max_frame_sync_attempts )
  {
    return false;
  }

  // Start with a blank msg buf -- this will short cycle the for loop
  // below if a message was not received
  memset (&(config_response_buf[0]), 0, GNSS_CONFIG_BUFFER_SIZE);

  // Send over the configuration settings
  hal_ret = HAL_UART_Transmit (self.gnss_uart_handle, config_array, message_size, uart_timeout);
  // Make sure the transmission went through completely
  if ( hal_ret != HAL_OK )
  {
    gnss_reset_uart ();
    return false;
  }

  hal_ret = HAL_UART_Receive_DMA (self.gnss_uart_handle, (uint8_t*) &(config_response_buf[0]),
  GNSS_CONFIG_BUFFER_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_uart4_rx, DMA_IT_HT);

  // Make sure we receive the response within the right amount of time
  if ( hal_ret != HAL_OK )
  {
    gnss_reset_uart ();
    return false;
  }

  elapsed_time = 0;
  start_time = HAL_GetTick ();
  while ( (elapsed_time < uart_timeout) && !rx_complete )
  {
    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( !rx_complete )
  {
//    gnss_reset_uart ();
    return false;
  }

  rx_complete = false;

  /* The ack/nak message is guaranteed to be sent within one second, but
   * we may receive a few navigation messages before the ack is received,
   * so we have to sift through at least one second worth of messages */
  for ( num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                                payload, sizeof(payload), &buf_end);
      num_payload_bytes > 0;
      num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                              payload, sizeof(payload), &buf_end) )
  {
    if ( message_class == 0x05 )
    {
      // Msg class 0x05 is either an ACK or NAK
      if ( message_id == 0x00 )
      {
        // This is a NAK msg, the config did not go through properly
        return false;
      }

      if ( message_id == 0x01 )
      {
        // This is an ACK message
        response_msg_class = payload[UBX_ACK_ACK_CLSID_INDEX];
        response_msg_id = payload[UBX_ACK_ACK_MSGID_INDEX];

        // Make sure this is an ack for the CFG_VALSET message type
        if ( response_msg_class == response_class && response_msg_id == response_id )
        {
          // This is an acknowledgement of our configuration message
//          gnss_reset_uart ();
          return true;
        }
      }
    }
    else if ( (message_class == UBX_NAV_PVT_MESSAGE_CLASS)
              && (message_id == UBX_NAV_PVT_MESSAGE_ID) )
    {
      return true;
    }
    // Adjust pointers to continue searching the buffer
    buf_length -= buf_end - buf_start;
    buf_start = buf_end;
  }

//  gnss_reset_uart ();
  return false;
}

/**
 *
 *
 * @param self- GNSS struct
 * @param
 */
static void process_frame_sync_messages ( uint8_t *process_buf )
{
  char payload[UBX_NAV_PVT_PAYLOAD_LENGTH];
  const char *buf_start = (const char*) &(process_buf[0]);
  const char *buf_end = buf_start;

  size_t buf_length = INITIAL_STAGES_BUFFER_SIZE;
  int32_t message_class = 0;
  int32_t message_id = 0;
  int32_t num_payload_bytes = 0;
  // Reset the counters
  self.messages_processed = 0;
  self.number_cycles_without_data = 0;
  self.total_samples = 0;

  // Really gross for loop that processes msgs in each iteration
  for ( num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                                payload, sizeof(payload), &buf_end);
      num_payload_bytes > 0;
      num_payload_bytes = uUbxProtocolDecode (buf_start, buf_length, &message_class, &message_id,
                                              payload, sizeof(payload), &buf_end) )
  {
    // UBX_NAV_PVT payload is 92 bytes, message class is 0x01,
    // message ID is 0x07
    if ( num_payload_bytes != UBX_NAV_PVT_PAYLOAD_LENGTH
         || message_class != UBX_NAV_PVT_MESSAGE_CLASS || message_id != UBX_NAV_PVT_MESSAGE_ID )
    {
      self.number_cycles_without_data++;
      buf_length -= buf_end - buf_start;
      buf_start = buf_end;
      continue;
    }

    // need to keep track of how many messages were processed in the buffer
    self.messages_processed++;
    self.number_cycles_without_data = 0;
    self.total_samples++;

    buf_length -= buf_end - buf_start;
    buf_start = buf_end;
  }
}

/**
 * Function that returns two bytes as an uint16_t from a byte array
 *
 * @param byte_array - pointer to byte array
 * @param start_index - the start index of the bytes to extract
 *
 * @return The two bytes as a uint16_t -- can be cast to different formats
 */
static uint16_t get_two_bytes ( uint8_t *byte_array, uint32_t start_index, endian_t endianess )
{
  uint16_t return_val = 0;

  if ( endianess == AS_BIG_ENDIAN )
  {
    return_val = ((byte_array[start_index] << 8) + (byte_array[start_index + 1]));
  }
  else
  {
    return_val = (byte_array[start_index] + (byte_array[start_index + 1] << 8));
  }

  return return_val;
}

void HAL_UART_RxCpltCallback ( UART_HandleTypeDef *huart )
{
  rx_complete = true;
}

void HAL_UARTEx_RxEventCallback ( UART_HandleTypeDef *huart, uint16_t Size )
{
  rx_complete = true;
  received_bytes = Size;
}
