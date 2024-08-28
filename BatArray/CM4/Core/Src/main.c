/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sdcard.h"
#include "configuration.h"
#include "gnss.h"
#include "time.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool ready_to_write = false;
bool done = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
static void _main_busy_loop ( uint32_t delay );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main ( void )
{

  /* USER CODE BEGIN 1 */
  uint32_t start_time = 0, elapsed_time = 0, gnss_config_timeout = 10000, gnss_sync_timeout = 60000,
      gnss_get_timeout = 60000;
  struct tm start_timestamp, stop_timestamp;
  /* USER CODE END 1 */

  /* USER CODE BEGIN Boot_Mode_Sequence_1 */
#ifndef DEBUG_CM4_STANDALONE
  /*HW semaphore Clock enable*/
  __HAL_RCC_HSEM_CLK_ENABLE();
  /* Activate HSEM notification for Cortex-M4*/
  HAL_HSEM_ActivateNotification (__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
  /*
   Domain D2 goes to STOP mode (Cortex-M4 in deep-sleep) waiting for Cortex-M7 to
   perform system initialization (system clock config, external memory configuration.. )
   */
  HAL_PWREx_ClearPendingEvent ();
  HAL_PWREx_EnterSTOPMode (PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFE, PWR_D2_DOMAIN);
  /* Clear HSEM flag */
  __HAL_HSEM_CLEAR_FLAG(__HAL_HSEM_SEMID_TO_MASK(HSEM_ID_0));
#endif // #ifndef DEBUG_CM4_STANDALONE
  /* USER CODE END Boot_Mode_Sequence_1 */
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init ();

  /* USER CODE BEGIN Init */

  HAL_HSEM_ActivateNotification (__HAL_HSEM_SEMID_TO_MASK(BUFFER_1_FULL_SEMAPHORE));
  HAL_HSEM_ActivateNotification (__HAL_HSEM_SEMID_TO_MASK(BUFFER_2_FULL_SEMAPHORE));
  HAL_HSEM_ActivateNotification (__HAL_HSEM_SEMID_TO_MASK(ERROR_SEMAPHORE));
  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */
  // Grab semaphore 1 to prevent M7 core from moving forward until we've gotten the file
  // system initialized
  HAL_HSEM_FastTake (M4_READY_SEMAPHORE);
  HAL_HSEM_FastTake (DONE_SEMAPHORE);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init ();
  MX_DMA_Init ();
  MX_UART4_Init ();
//  MX_SDMMC2_SD_Init();
  MX_USART6_UART_Init ();
//  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  gnss_init ();

  start_time = HAL_GetTick ();

  while ( elapsed_time < gnss_config_timeout )
  {
    if ( gnss_config () )
    {
      break;
    }

    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( elapsed_time >= gnss_config_timeout )
  {
    Error_Handler ();
  }

  start_time = HAL_GetTick ();
  elapsed_time = 0;

  while ( elapsed_time < gnss_sync_timeout )
  {
    if ( gnss_sync () )
    {
      break;
    }

    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( elapsed_time >= gnss_sync_timeout )
  {
    Error_Handler ();
  }

  start_time = HAL_GetTick ();
  elapsed_time = 0;

  while ( elapsed_time < gnss_get_timeout )
  {
    if ( gnss_get_time (&start_timestamp) )
    {
      break;
    }

    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( elapsed_time >= gnss_get_timeout )
  {
    Error_Handler ();
  }

  MX_SDMMC2_SD_Init ();
  MX_FATFS_Init ();

  if ( !sdcard_mount () )
  {
    Error_Handler ();
  }

  /*
   * Create a bookkeeping file:
   * Put start/ stop times
   * Total number of samples
   * Other things?
   */

  if ( !sdcard_allocate_files () )
  {
    Error_Handler ();
  }

  HAL_HSEM_Release (M4_READY_SEMAPHORE, 0);

  HAL_Delay (1);
  ready_to_write = false;

  while ( !done )
  {
    if ( ready_to_write )
    {
      ready_to_write = false;

      if ( !sdcard_write_to_file () )
      {
        Error_Handler ();
      }
    }
  }

  start_time = HAL_GetTick ();
  elapsed_time = 0;

  while ( elapsed_time < gnss_get_timeout )
  {
    if ( gnss_get_time (&stop_timestamp) )
    {
      break;
    }

    elapsed_time = HAL_GetTick () - start_time;
  }

  if ( elapsed_time >= gnss_get_timeout )
  {
    Error_Handler ();
  }

  if ( !sdcard_write_start_stop_times (&start_timestamp, &stop_timestamp) )
  {
    Error_Handler ();
  }

  sdcard_shutdown ();

  supplemental_gpio_init ();
  // Signal to M7 that we are done here
  HAL_HSEM_Release (DONE_SEMAPHORE, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  bool toggle = true;
  while ( 1 )
  {
    _main_busy_loop (5000000);
    if ( toggle )
    {
      green_led_on ();
    }
    else
    {
      green_led_off ();
    }
    toggle = !toggle;
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
static void _main_busy_loop ( uint32_t delay )
{
  volatile int dummy;

  for ( int i = 0; i < delay; i++ )
  {
    dummy = i;
    i = dummy;
    dummy = 1 << (i % 32);
  }
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler ( void )
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  sdcard_shutdown ();

  HAL_HSEM_Release (DONE_SEMAPHORE, 0);

  HAL_HSEM_FastTake (ERROR_SEMAPHORE);

  supplemental_gpio_init ();

  green_led_off ();

  bool toggle = true;

  while ( 1 )
  {
    _main_busy_loop (5000000);

    if ( toggle )
    {
      red_led_on ();
    }
    else
    {
      red_led_off ();
    }

    toggle = !toggle;
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed ( uint8_t *file, uint32_t line )
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
