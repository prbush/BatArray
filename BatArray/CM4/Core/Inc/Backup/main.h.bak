/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_nucleo.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern bool ready_to_write;
extern bool done;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define M4_READY_SEMAPHORE (1U)
#define BUFFER_1_FULL_SEMAPHORE (2U)
#define BUFFER_2_FULL_SEMAPHORE (3U)
#define DONE_SEMAPHORE (4U)
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler ( void );

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SD_D0_Pin GPIO_PIN_14
#define SD_D0_GPIO_Port GPIOB
#define SD_D1_Pin GPIO_PIN_15
#define SD_D1_GPIO_Port GPIOB
#define SD_CK_Pin GPIO_PIN_6
#define SD_CK_GPIO_Port GPIOD
#define SD_CMD_Pin GPIO_PIN_7
#define SD_CMD_GPIO_Port GPIOD
#define SDMMC_CD_Pin GPIO_PIN_10
#define SDMMC_CD_GPIO_Port GPIOG
#define SD_D2_Pin GPIO_PIN_3
#define SD_D2_GPIO_Port GPIOB
#define SD_D3_Pin GPIO_PIN_4
#define SD_D3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define SDMMC_CD_Pin GPIO_PIN_10
#define SDMMC_CD_GPIO_Port GPIOG
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
