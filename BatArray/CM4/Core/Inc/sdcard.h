/*
 * sdcard.h
 *
 *  Created on: Jul 10, 2024
 *      Author: philbush
 */

#ifndef INC_SDCARD_H_
#define INC_SDCARD_H_

#include "fatfs.h"
#include "stdbool.h"
#include "time.h"

extern uint32_t file_array_index;
extern uint32_t buffer_select;

#define ONE_GB (1073741824U)
#define CLUSTER_SIZE_SAMSUNG_512 (262144U)
#define RAW_VAL_BUFFER_SIZE (131072U)

typedef struct
{
  uint8_t conversion_buffer[2][RAW_VAL_BUFFER_SIZE];
} ad7606c_data_buffer;

bool sdcard_mount ( void );
void sdcard_shutdown ( void );
bool sdcard_allocate_files ( void );
bool sdcard_write_to_file ( void );
bool sdcard_write_start_stop_times ( struct tm *start, struct tm *stop );

#endif /* INC_SDCARD_H_ */
