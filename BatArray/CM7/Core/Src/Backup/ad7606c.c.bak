/*
 * ad7606c.c
 *
 *  Created on: Jul 3, 2024
 *      Author: philbush
 */

#include "ad7606c.h"

#include "string.h" // memset
#include "hrtim.h"
#include "main.h"
#include "stm32h7xx_it.h"
#include "configuration.h"

//__attribute__((section(".DTCMRAM_sec")))                     static ad7606c ad7606c_instance;
static ad7606c ad7606c_instance;
static float channel_1_conversion = 0;
__attribute__((section(".ADC_BUFFER_sec")))                             ad7606c_data_buffer data_buffer;
uint32_t buffer_index = 0;
uint32_t buffer_array_index = 0;

static void _ad7606c_read_reg ( uint8_t reg_addr, uint8_t *reg_data );
static void _ad7606c_write_reg ( uint8_t reg_addr, uint8_t reg_data );
static void _ad7606c_exit_register_mode ( void );
static void _ad7606c_busy_loop ( uint32_t delay );

// Helper functions for aux line bit toggling
static inline void _ad7606c_toggle_convst ( void );
static inline void _ad7606c_pull_cs_rd_low ( void );
static inline void _ad7606c_push_cs_rd_high ( void );

bool ad7606c_init ( void )
{
  ad7606c_aux_op aux_lines;
  uint8_t id = 0;

  ad7606c_instance.data_lines_input_struct.Pin = GPIO_PIN_All;
  ad7606c_instance.data_lines_input_struct.Mode = GPIO_MODE_INPUT;
  ad7606c_instance.data_lines_input_struct.Pull = GPIO_NOPULL;

  ad7606c_instance.data_lines_output_struct.Pin = GPIO_PIN_All;
  ad7606c_instance.data_lines_output_struct.Mode = GPIO_MODE_OUTPUT_PP;
  ad7606c_instance.data_lines_output_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  ad7606c_instance.data_lines_output_struct.Pull = GPIO_NOPULL;

  ad7606c_instance.aux_lines_output_struct.Pin = ADC_RESET_PIN | ADC_CONVST_PIN | ADC_RD_PIN
                                                 | ADC_WR_PIN | ADC_CS_PIN;
  ad7606c_instance.aux_lines_output_struct.Mode = GPIO_MODE_OUTPUT_PP;
  ad7606c_instance.aux_lines_output_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  ad7606c_instance.aux_lines_output_struct.Pull = GPIO_NOPULL;

  ad7606c_instance.aux_lines_input_struct.Pin = ADC_BUSY_PIN;
  ad7606c_instance.aux_lines_input_struct.Mode = GPIO_MODE_IT_FALLING;
  ad7606c_instance.aux_lines_input_struct.Pull = GPIO_NOPULL;

  ad7606c_instance.data_lines_gpio_port = GPIOE;
  ad7606c_instance.aux_lines_gpio_port = GPIOF;

  ad7606c_instance.aux_rd_gpio_pin = ADC_RESET_PIN;
  ad7606c_instance.aux_busy_gpio_pin = ADC_BUSY_PIN;
  ad7606c_instance.aux_convst_gpio_pin = ADC_CONVST_PIN;
  ad7606c_instance.aux_rd_gpio_pin = ADC_RD_PIN;
  ad7606c_instance.aux_wr_gpio_pin = ADC_WR_PIN;
  ad7606c_instance.aux_cs_gpio_pin = ADC_CS_PIN;

  ad7606c_instance.conversions_processed = 0;

  memset (&data_buffer, 0, sizeof(ad7606c_data_buffer));

  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  // Init the GPIO ports
  HAL_GPIO_Init (ad7606c_instance.data_lines_gpio_port, &ad7606c_instance.data_lines_input_struct);
  HAL_GPIO_Init (ad7606c_instance.aux_lines_gpio_port, &ad7606c_instance.aux_lines_output_struct);
  HAL_GPIO_Init (ad7606c_instance.aux_lines_gpio_port, &ad7606c_instance.aux_lines_input_struct);

// Setup the aux pins
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 1;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 1;
  aux_lines.aux_pins.cs = 1;
  aux_lines.aux_pins.rst = 1;
  aux_lines.aux_pins.unused_top = 0;

  HAL_Delay (10);

  // Apply Aux pins -- will reset ADC
  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (2560);
  // Release reset pin
  aux_lines.aux_pins.rst = 0;
  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;
  HAL_Delay (10);

#ifndef SKIP_ADC_CONFIG_CHECKS
  _ad7606c_read_reg (ID_REG, &id);

  if ( id != AD7606C_ID )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  return true;

}

void ad7606c_shutdown ()
{
  (void) HAL_HRTIM_SimpleBaseStop_IT (&hhrtim, HRTIM_TIMERINDEX_MASTER);
  (void) HAL_HRTIM_DeInit (&hhrtim);
}

bool ad7606c_apply_settings ( void )
{
  uint8_t write_reg = 0;
  uint8_t read_reg = 0xff;

  // Config register: normal operating mode
  write_reg = 0b00000000;

  _ad7606c_write_reg ( CONFIG_REG, write_reg);
  _ad7606c_read_reg ( CONFIG_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  read_reg = 0xff; // reset read data

  write_reg = 0b10001000; // Range Registers: +/- 5V differential for both channels

  _ad7606c_write_reg ( RANGE_CH1_CH2_REG, write_reg);
  _ad7606c_read_reg ( RANGE_CH1_CH2_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  read_reg = 0xff; // reset read data

  _ad7606c_write_reg ( RANGE_CH3_CH4_REG, write_reg);
  _ad7606c_read_reg ( RANGE_CH3_CH4_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  read_reg = 0xff; // reset read data

  _ad7606c_write_reg ( RANGE_CH5_CH6_REG, write_reg);
  _ad7606c_read_reg ( RANGE_CH5_CH6_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  read_reg = 0xff; // reset read data

  _ad7606c_write_reg ( RANGE_CH7_CH8_REG, write_reg);
  _ad7606c_read_reg ( RANGE_CH7_CH8_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  read_reg = 0x00; // reset read data

  write_reg = 0xff; // Bandwidth register: enable high bandwidth on all channels

  _ad7606c_write_reg ( BANDWIDTH_REG, write_reg);
  _ad7606c_read_reg ( BANDWIDTH_REG, &read_reg);

#ifndef SKIP_ADC_CONFIG_CHECKS
  if ( read_reg != write_reg )
  {
    return false;
  }
#endif // #ifndef SKIP_ADC_CONFIG_CHECKS

  // Exit and return to ADC mode
  _ad7606c_exit_register_mode ();

  return true;
}

bool ad7606c_start_conversion ( void )
{
//  GPIO_InitTypeDef busy_pin_config;
  // Setup the high resolution timer in interrupot mode
  if ( HAL_HRTIM_SimpleBaseStart_IT (&hhrtim, HRTIM_TIMERINDEX_MASTER) != HAL_OK )
  {
    return false;
  }

  // Enable BUSY pin falling edge interrupt
//  HAL_NVIC_SetPriority (EXTI9_5_IRQn, 1, 1);
//  HAL_NVIC_EnableIRQ (EXTI9_5_IRQn);

  return true;
}

void ad7606c_read_conversion_results ( void )
{
  uint16_t *data_lines_idr_reg = (uint32_t*) 0x58021010; // GPIOE IDR
  uint32_t *aux_lines_odr_reg = (uint32_t*) 0x58021414; // GPIOF ODR
  uint32_t *aux_lines_idr_reg = (uint32_t*) 0x58021410; // GPIOF ODR
  static uint64_t last_timer_irq_counter = 0;
  uint16_t buf[8];

  HAL_HSEM_Take (BUFFER_1_FULL_SEMAPHORE, 0);

  // Let the timer settle a bit
  while ( timer_irq_counter < ADC_SAMPLING_RATE );

  timer_irq_counter = 0;

  while ( !done )
  {
    // Once the timer counter increments
    if ( last_timer_irq_counter != timer_irq_counter )
    {
      last_timer_irq_counter = timer_irq_counter;

      // wait for the busy pin to fall
      while ( *aux_lines_idr_reg & ADC_BUSY_PIN );

      for ( int i = 0; i < 8; i++ )
      {
        // Pull RD and CS low
        *aux_lines_odr_reg = 0x280;

        buf[i] = *data_lines_idr_reg;

        // Push RD and CS high
        *aux_lines_odr_reg = 0x780;
      }

      ad7606c_instance.conversions_processed++;

      // Convert a channel for debugging
      channel_1_conversion = (int16_t) buf[0] * 0.000152588f;

      // Copy the results over
      memcpy (&(data_buffer.conversion_buffer[buffer_array_index][buffer_index]), &(buf[0]),
              sizeof(buf));

      buffer_index += sizeof(buf);

      // Swap buffers if necessary
      if ( buffer_index == RAW_VAL_BUFFER_SIZE )
      {

        // Force dirty write backs to memory
//        SCB_CleanDCache ();

        if ( buffer_array_index == 0 )
        {
          HAL_HSEM_Release (BUFFER_1_FULL_SEMAPHORE, 0);
          buffer_array_index = 1;
          buffer_index = 0;
          HAL_HSEM_Take (BUFFER_2_FULL_SEMAPHORE, 0);
        }
        else
        {
          HAL_HSEM_Release (BUFFER_2_FULL_SEMAPHORE, 0);
          buffer_array_index = 0;
          buffer_index = 0;
          HAL_HSEM_Take (BUFFER_1_FULL_SEMAPHORE, 0);
        }
      }
    }
  }
}

static void _ad7606c_read_reg ( uint8_t reg_addr, uint8_t *reg_data )
{
  ad7606c_reg_op data_lines;
  ad7606c_aux_op aux_lines;
  ad7606c_reg_op read_bits;

// Init the GPIO ports to output mode
  HAL_GPIO_Init (ad7606c_instance.data_lines_gpio_port, &ad7606c_instance.data_lines_output_struct);
  HAL_GPIO_Init (ad7606c_instance.aux_lines_gpio_port, &ad7606c_instance.aux_lines_output_struct);

// Setup the data lines for a write
  data_lines.read_reg.db2_9_dont_care = 0x00;
  data_lines.read_reg.db10_16_reg_addr = reg_addr;
  data_lines.read_reg.bd17_rd_wr = ADC_READ_BIT;

// Setup the aux lines
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 1;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 0;
  aux_lines.aux_pins.cs = 0;
  aux_lines.aux_pins.rst = 0;
  aux_lines.aux_pins.unused_top = 0;

  ad7606c_instance.data_lines_gpio_port->ODR = data_lines.bytes;
  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (2560);

  aux_lines.aux_pins.cs = 1;
  aux_lines.aux_pins.wr = 1;

  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

// Switch data lines to input for a read
  ad7606c_instance.data_lines_input_struct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init (ad7606c_instance.data_lines_gpio_port, &ad7606c_instance.data_lines_input_struct);

  aux_lines.aux_pins.cs = 0;
  aux_lines.aux_pins.rd = 0;

  _ad7606c_busy_loop (256);

  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (256);

// Read the bits
  read_bits.bytes = ad7606c_instance.data_lines_gpio_port->IDR;

// Cut the address off and return just the data
  *reg_data = read_bits.write_reg.db2_9_reg_data;

  _ad7606c_busy_loop (2560);

// Reset aux pins
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 1;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 1;
  aux_lines.aux_pins.cs = 1;
  aux_lines.aux_pins.rst = 0;
  aux_lines.aux_pins.unused_top = 0;

  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (2560);

}

static void _ad7606c_write_reg ( uint8_t reg_addr, uint8_t reg_data )
{
  ad7606c_reg_op data_lines;
  ad7606c_aux_op aux_lines;

// Init the GPIO ports to output mode
  HAL_GPIO_Init (ad7606c_instance.data_lines_gpio_port, &ad7606c_instance.data_lines_output_struct);
  HAL_GPIO_Init (ad7606c_instance.aux_lines_gpio_port, &ad7606c_instance.aux_lines_output_struct);

// Setup the data lines for a write
  data_lines.write_reg.db2_9_reg_data = reg_data;
  data_lines.write_reg.db10_16_reg_addr = reg_addr;
  data_lines.write_reg.bd17_rd_wr = ADC_WRITE_BIT;

// Setup the aux lines
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 0;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 0;
  aux_lines.aux_pins.cs = 0;
  aux_lines.aux_pins.rst = 0;
  aux_lines.aux_pins.unused_top = 0;

  ad7606c_instance.data_lines_gpio_port->ODR = data_lines.bytes;
  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (256);

// Reset aux pins
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 1;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 1;
  aux_lines.aux_pins.cs = 1;
  aux_lines.aux_pins.rst = 0;
  aux_lines.aux_pins.unused_top = 0;

  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

  _ad7606c_busy_loop (256);
}

static void _ad7606c_exit_register_mode ( void )
{
  ad7606c_aux_op aux_lines;

  _ad7606c_write_reg (0x00, 0x00);

// Reset the aux pins
  aux_lines.aux_pins.unused_bottom = 0;
  aux_lines.aux_pins.convst = 1;
  aux_lines.aux_pins.rd = 1;
  aux_lines.aux_pins.wr = 1;
  aux_lines.aux_pins.cs = 1;
  aux_lines.aux_pins.rst = 0;
  aux_lines.aux_pins.unused_top = 0;

  ad7606c_instance.aux_lines_gpio_port->ODR = aux_lines.bytes;

// Switch data lines back to input
  ad7606c_instance.data_lines_input_struct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init (ad7606c_instance.data_lines_gpio_port, &ad7606c_instance.data_lines_input_struct);

  _ad7606c_busy_loop (2560);
}

static void _ad7606c_busy_loop ( uint32_t delay )
{
  volatile int dummy;

  for ( int i = 0; i < delay; i++ )
  {
    dummy = i;
    i = dummy;
    dummy = 1 << (i % 32);
  }
}

static inline void _ad7606c_toggle_convst ( void )
{
  uint32_t *aux_lines_odr_reg = (uint32_t*) 0x58021414;
  // Pull CONVST low and back high to start a conversion
  *aux_lines_odr_reg = 0x700;
  *aux_lines_odr_reg = 0x780;
}

static inline void _ad7606c_pull_cs_rd_low ( void )
{
  uint32_t *aux_lines_odr_reg = (uint32_t*) 0x58021414;
  *aux_lines_odr_reg = 0x280;
}

static inline void _ad7606c_push_cs_rd_high ( void )
{
  uint32_t *aux_lines_odr_reg = (uint32_t*) 0x58021414;
  *aux_lines_odr_reg = 0x780;
}

