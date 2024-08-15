/*
 * ad7606c.h
 *
 *  Created on: Jul 3, 2024
 *      Author: philbush
 */

#ifndef INC_AD7606C_H_
#define INC_AD7606C_H_

#include "gpio.h"
#include "stdbool.h"

// Registers
#define STATUS_REG 0X01
#define CONFIG_REG 0X02
#define RANGE_CH1_CH2_REG 0X03
#define RANGE_CH3_CH4_REG 0X04
#define RANGE_CH5_CH6_REG 0X05
#define RANGE_CH7_CH8_REG 0X06
#define BANDWIDTH_REG 0X07
#define OVERSAMPLING_REG 0X08
#define CH1_GAIN_REG 0X09
#define CH2_GAIN_REG 0X0A
#define CH3_GAIN_REG 0X0B
#define CH4_GAIN_REG 0X0C
#define CH5_GAIN_REG 0X0D
#define CH6_GAIN_REG 0X0E
#define CH7_GAIN_REG 0X0F
#define CH8_GAIN_REG 0X10
#define CH1_OFFSET_REG 0X11
#define CH2_OFFSET_REG 0X12
#define CH3_OFFSET_REG 0X13
#define CH4_OFFSET_REG 0X14
#define CH5_OFFSET_REG 0X15
#define CH6_OFFSET_REG 0X16
#define CH7_OFFSET_REG 0X17
#define CH8_OFFSET_REG 0X18
#define CH1_PHASE_REG 0X19
#define CH2_PHASE_REG 0X1A
#define CH3_PHASE_REG 0X1B
#define CH4_PHASE_REG 0X1C
#define CH5_PHASE_REG 0X1D
#define CH6_PHASE_REG 0X1E
#define CH7_PHASE_REG 0X1F
#define CH8_PHASE_REG 0X20
#define DIGITAL_DIAG_ENABLE_REG 0X21
#define DIGITAL_DIAG_ERR_REG 0X22
#define OPEN_DETECT_ENABLE_REG 0X23
#define OPEN_DETECRTED_REG 0X24
#define DIAGNOSTIC_MUX_CH1_CH2_REG 0X28
#define DIAGNOSTIC_MUX_CH3_CH4_REG 0X29
#define DIAGNOSTIC_MUX_CH5_CH6_REG 0X2A
#define DIAGNOSTIC_MUX_CH7_CH8_REG 0X2B
#define OPEN_DETECT_QUEUE_REG 0X2C
#define FS_CLK_COUNTER_REG 0X2D
#define OS_CLK_COUNTER_REG 0X2E
#define ID_REG 0X2F

// Aux Pins
#define ADC_RESET_PIN (1 << 11) // PF11
#define ADC_BUSY_PIN (1 << 6) // PF6
#define ADC_CONVST_PIN (1 << 7) // PF7
#define ADC_RD_PIN (1 << 8) // PF8
#define ADC_WR_PIN ( 1 << 9) // PF9
#define ADC_CS_PIN ( 1 << 10) // PF10

// Data pins
//#define DB0_PIN (1 << 0)
//#define DB1_PIN (1 << 1)
#define DB2_pin (1 << 0)
#define DB3_pin (1 << 1)
#define DB4_pin (1 << 2)
#define DB5_pin (1 << 3)
#define DB6_pin (1 << 4)
#define DB7_pin (1 << 5)
#define DB8_pin (1 << 6)
#define DB9_pin (1 << 7)
#define DB10_pin (1 << 8)
#define DB11_pin (1 << 9)
#define DB12_pin (1 << 10)
#define DB13_pin (1 << 11)
#define DB14_pin (1 << 12)
#define DB15_pin (1 << 13)
#define DB16_pin (1 << 14)
#define DB17_pin (1 << 15)

// Misc
#define ADC_READ_BIT 1
#define ADC_WRITE_BIT 0
#define AD7606C_ID 0X22
#define RAW_VAL_BUFFER_SIZE 131072 // 128K

typedef struct
{
  uint8_t db2_9_reg_data :8;
  uint8_t db10_16_reg_addr :7;
  uint8_t bd17_rd_wr :1;
} ad7606c_write_reg_t;

typedef struct
{
  uint8_t db2_9_dont_care :8;
  uint8_t db10_16_reg_addr :7;
  uint8_t bd17_rd_wr :1;
} ad7606c_read_reg_t;

typedef union
{
  ad7606c_read_reg_t read_reg;
  ad7606c_write_reg_t write_reg;
  uint16_t bytes;
} ad7606c_reg_op;

typedef struct
{
  uint8_t unused_bottom :7;
  uint8_t convst :1;
  uint8_t rd :1;
  uint8_t wr :1;
  uint8_t cs :1;
  uint8_t rst :1;
  uint8_t unused_top :5;
} ad7606c_aux_t;

typedef union
{
  ad7606c_aux_t aux_pins;
  uint16_t bytes;
} ad7606c_aux_op;

typedef struct
{
  uint8_t conversion_buffer[2][RAW_VAL_BUFFER_SIZE];
} ad7606c_data_buffer;

typedef struct
{
  GPIO_InitTypeDef data_lines_input_struct;
  GPIO_InitTypeDef data_lines_output_struct;
  GPIO_InitTypeDef aux_lines_output_struct;
  GPIO_InitTypeDef aux_lines_input_struct;
  GPIO_TypeDef *data_lines_gpio_port;
  GPIO_TypeDef *aux_lines_gpio_port;

  uint16_t aux_reset_gpio_pin;
  uint16_t aux_busy_gpio_pin;
  uint16_t aux_convst_gpio_pin;
  uint16_t aux_rd_gpio_pin;
  uint16_t aux_wr_gpio_pin;
  uint16_t aux_cs_gpio_pin;

  uint64_t conversions_processed;

} ad7606c;

bool ad7606c_init ( void );
void ad7606c_shutdown ( void );
bool ad7606c_apply_settings ( void );
bool ad7606c_start_conversion ( void );
void ad7606c_read_conversion_results ( void );

#endif /* INC_AD7606C_H_ */
