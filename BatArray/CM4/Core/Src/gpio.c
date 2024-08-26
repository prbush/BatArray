/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
     PB15   ------> SDMMC2_D1
     PD6   ------> SDMMC2_CK
     PD7   ------> SDMMC2_CMD
     PB3 (JTDO/TRACESWO)   ------> SDMMC2_D2
     PB4 (NJTRST)   ------> SDMMC2_D3
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pins : PB15 PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF9_SDIO2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF11_SDIO2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PG10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
void supplemental_gpio_init ( void )
{
  GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };
  /*Configure GPIO pin : Green LED --> PB0, Red LED --> PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init (GPIOB, &GPIO_InitStruct);

  green_led_off ();
  red_led_off ();
}

void green_led_on ( void )
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void green_led_off ( void )
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

void red_led_on ( void )
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
}

void red_led_off ( void )
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
}
/* USER CODE END 2 */
