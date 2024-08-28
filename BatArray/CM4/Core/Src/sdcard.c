/*
 * sdcard.c
 *
 *  Created on: Jul 10, 2024
 *      Author: philbush
 */

#include "sdcard.h"
#include "string.h"
#include "main.h"
#include "sdmmc.h"
#include "configuration.h"

FIL file_array[NUMBER_OF_FILES];
FIL start_stop_times_file;
uint32_t writes_counter = 0;
uint32_t file_array_index = 0;
uint32_t buffer_select = 0;
__attribute__((section(".ADC_BUFFER_sec")))           ad7606c_data_buffer data_buffer;

uint32_t error_line = 0;
FRESULT error_result = FR_OK;
uint64_t seek_point = 0;
uint32_t num_file_writes = 0;
static uint8_t work[16384];

static uint64_t get_file_size ( void );
static uint32_t get_max_writes_per_file ( void );

bool sdcard_mount ( void )
{
  uint8_t work[16384];
  // Format the card
  if ( f_mkfs ("", FM_EXFAT, CLUSTER_SIZE_SAMSUNG_512, &(work[0]), sizeof(work)) != FR_OK )
  {

    error_line = 34;
    return false;
  }

  // Mount the SD card
  return f_mount (&SDFatFS, "", 1) == FR_OK;
}

bool sdcard_write_start_stop_times ( time_t start, time_t stop )
{
  FRESULT res;
  UINT bytes_written = 0;
  uint8_t buffer[sizeof(time_t) * 2];

  memcpy (&buffer[0], &start, sizeof(time_t));
  memcpy (&buffer[sizeof(time_t)], &stop, sizeof(time_t));

  res = f_lseek (&start_stop_times_file, 0);
  if ( res != FR_OK )
  {
    error_result = res;
    error_line = 54;

    return false;
  }

  res = f_write (&start_stop_times_file, (const void*) &(buffer[0]), sizeof(buffer),
                 &bytes_written);

  if ( (res != FR_OK) || (bytes_written != RAW_VAL_BUFFER_SIZE) )
  {
    error_result = res;
    error_line = 63;

    return false;
  }

  res = f_close (&start_stop_times_file);
  if ( res != FR_OK )
  {
    error_result = res;
    error_line = 74;
    return false;
  }

  return true;
}

void sdcard_shutdown ( void )
{
  // Deinit SDMMC2
  (void) HAL_SD_DeInit (&hsd2);
}

bool sdcard_allocate_files ( void )
{
  FRESULT res;
  // 1 Hour's worth of data
  uint64_t expand_size = get_file_size ();
  char filename_buffer[32];

  // Bookeeping file
  snprintf (filename_buffer, 32, "Start_Stop_Times");
  res = f_open (&start_stop_times_file, filename_buffer, FA_CREATE_ALWAYS | FA_WRITE);
  if ( res != FR_OK )
  {
    error_result = res;
    error_line = 99;

    return false;
  }
  res = f_expand (&start_stop_times_file, 512, 1);
  if ( res != FR_OK )
  {
    error_result = res;
    error_line = 107;

    return false;
  }

  // Create the 12 hourly files
  for ( int i = 0; i < NUMBER_OF_FILES; i++ )
  {
    snprintf (filename_buffer, 32, "hour_%d.raw", i);
    // Open the first file
    res = f_open (&file_array[i], filename_buffer, FA_CREATE_ALWAYS | FA_WRITE);
    if ( res != FR_OK )
    {
      error_result = res;
      error_line = 121;

      return false;
    }

    res = f_expand (&file_array[i], expand_size, 1);
    if ( res != FR_OK )
    {
      error_result = res;
      error_line = 130;

      return false;
    }
  }

  res = f_lseek (&file_array[0], 0);

  return res == FR_OK;
}

bool sdcard_write_to_file ( void )
{
  FRESULT res;
  uint8_t *ptr = &(data_buffer.conversion_buffer[buffer_select][0]);
  UINT bytes_written = 0;
  uint32_t max_writes_to_file = get_max_writes_per_file ();

  res = f_write (&file_array[file_array_index], (const void*) ptr, RAW_VAL_BUFFER_SIZE,
                 &bytes_written);

  if ( (res != FR_OK) || (bytes_written != RAW_VAL_BUFFER_SIZE) )
  {
    error_result = res;
    error_line = 155;

    return false;
  }

  num_file_writes++;

  writes_counter++;

  // Check for file full
  if ( num_file_writes == max_writes_to_file )
  {
    // Close the file, switch to the next one
    res = f_close (&file_array[file_array_index]);

    if ( res != FR_OK )
    {
      error_result = res;
      error_line = 171;

      return false;
    }

    file_array_index++;

    if ( file_array_index == NUMBER_OF_FILES )
    {
      // Jump out of the loop
      done = true;
      return true;
    }

    num_file_writes = 0;
  }

  seek_point = (uint64_t) num_file_writes * (uint64_t) RAW_VAL_BUFFER_SIZE;

  res = f_lseek (&file_array[file_array_index], seek_point);

  if ( res != FR_OK )
  {
    error_result = res;
    error_line = 195;

    return false;
  }

  return true;
}

static uint64_t get_file_size ( void )
{
  uint64_t file_size = (((uint64_t) ADC_SAMPLING_RATE) * 16ULL * 60ULL * ((uint64_t) MINS_PER_FILE));
  file_size += (uint64_t) (((uint64_t) RAW_VAL_BUFFER_SIZE)
                           - (file_size % ((uint64_t) RAW_VAL_BUFFER_SIZE)));

  return file_size;
}

static uint32_t get_max_writes_per_file ( void )
{
  return (uint32_t) (get_file_size () / ((uint64_t) RAW_VAL_BUFFER_SIZE));
}
