/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LD4_Pin GPIO_PIN_0
#define LD4_GPIO_Port GPIOF
#define LD5_Pin GPIO_PIN_2
#define LD5_GPIO_Port GPIOF
#define Joystick_Button_Pin GPIO_PIN_6
#define Joystick_Button_GPIO_Port GPIOA
#define Joystick_Button_EXTI_IRQn EXTI9_5_IRQn
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#define LD6_Pin GPIO_PIN_13
#define LD6_GPIO_Port GPIOF
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define ENCODER_A_Pin GPIO_PIN_5
#define ENCODER_A_GPIO_Port GPIOG
#define ENCODER_A_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_BUTTON_Pin GPIO_PIN_7
#define ENCODER_BUTTON_GPIO_Port GPIOG
#define ENCODER_BUTTON_EXTI_IRQn EXTI9_5_IRQn
#define ENCODER_B_Pin GPIO_PIN_8
#define ENCODER_B_GPIO_Port GPIOG
#define ENCODER_B_EXTI_IRQn EXTI9_5_IRQn
#define LCD_Key2_Pin GPIO_PIN_10
#define LCD_Key2_GPIO_Port GPIOC
#define LCD_Key3_Pin GPIO_PIN_11
#define LCD_Key3_GPIO_Port GPIOC
#define LCD_Key0_Pin GPIO_PIN_12
#define LCD_Key0_GPIO_Port GPIOC
#define LCD_BackLight_Pin GPIO_PIN_6
#define LCD_BackLight_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
