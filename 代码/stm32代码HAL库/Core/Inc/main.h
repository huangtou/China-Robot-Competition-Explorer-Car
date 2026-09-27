/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sys.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define JG1_Pin GPIO_PIN_2
#define JG1_GPIO_Port GPIOE
#define JG2_Pin GPIO_PIN_3
#define JG2_GPIO_Port GPIOE
#define QC1_Pin GPIO_PIN_13
#define QC1_GPIO_Port GPIOC
#define GD1_Pin GPIO_PIN_6
#define GD1_GPIO_Port GPIOA
#define GD2_Pin GPIO_PIN_7
#define GD2_GPIO_Port GPIOA
#define GD5_Pin GPIO_PIN_4
#define GD5_GPIO_Port GPIOC
#define GD6_Pin GPIO_PIN_5
#define GD6_GPIO_Port GPIOC
#define GD7_Pin GPIO_PIN_0
#define GD7_GPIO_Port GPIOB
#define GD8_Pin GPIO_PIN_1
#define GD8_GPIO_Port GPIOB
#define CCD1_CLK_Pin GPIO_PIN_0
#define CCD1_CLK_GPIO_Port GPIOG
#define CCD1_SI_Pin GPIO_PIN_1
#define CCD1_SI_GPIO_Port GPIOG
#define CCD2_CLK_Pin GPIO_PIN_7
#define CCD2_CLK_GPIO_Port GPIOE
#define CCD2_SI_Pin GPIO_PIN_8
#define CCD2_SI_GPIO_Port GPIOE
#define KEY_LEFT_Pin GPIO_PIN_8
#define KEY_LEFT_GPIO_Port GPIOD
#define KEY_MID_Pin GPIO_PIN_9
#define KEY_MID_GPIO_Port GPIOD
#define KEY_RIGHT_Pin GPIO_PIN_10
#define KEY_RIGHT_GPIO_Port GPIOD
#define KEY1_Pin GPIO_PIN_11
#define KEY1_GPIO_Port GPIOD
#define HSL_SDA_Pin GPIO_PIN_2
#define HSL_SDA_GPIO_Port GPIOD
#define HSL_CLK_Pin GPIO_PIN_3
#define HSL_CLK_GPIO_Port GPIOD
#define TLY_SDA_Pin GPIO_PIN_0
#define TLY_SDA_GPIO_Port GPIOE
#define TLY_SCL_Pin GPIO_PIN_1
#define TLY_SCL_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
