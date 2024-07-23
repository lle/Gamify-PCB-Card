/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define GAME_SNAKE_ID 0
#define GAME_RACE_ID 1
#define GAME_ANIM_ID 2
#define MAIN_MENU_ID 3
#define MAX_ANIM_ID 10
#define DELAY_PER_LETTER 500

#include "screenBuffer.h"
#include "ascii_letter.h"
#include "game_snake.h"
#include "game_racer.h"
#include "anim_random.h"
#include "anim_swipe.h"
#include "anim_name.h"
#include "anim_ball.h"
#include "anim_life.h"

uint8_t GAME_MODE = 0;
uint8_t new_game_select = 0;
uint8_t currentAnimation = 2;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void main_menu()
//{
//	if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET)	//left
//	{
//		if(new_game_select == 0)
//			new_game_select = 2;	//wrap-around
//		else
//			new_game_select--;
//		screen_show_letter(new_game_select + ASCII_ONE);	//show the game ID between 1 and 3
//		HAL_Delay(250);
//	}
//
//	if(HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET)	//right
//	{
//		new_game_select++;
//		if(new_game_select == 3)
//			new_game_select = 0; //wrap-around
//
//		screen_show_letter(new_game_select + ASCII_ONE);	//show the game ID between 1 and 3
//		HAL_Delay(250);
//	}
//
//	if(HAL_GPIO_ReadPin(BTA_GPIO_Port, BTA_Pin) == GPIO_PIN_RESET)	//Confirm new game
//	{
//		GAME_MODE = new_game_select;
//	}
//}

void game_snake()
{
	snake_runGame();

	if(HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == GPIO_PIN_RESET)
	  snake_set_new_direction(GOING_UP);

	else if(HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin) == GPIO_PIN_RESET)
	  snake_set_new_direction(GOING_DOWN);

	else if(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET)
	  snake_set_new_direction(GOING_LEFT);

	else if(HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET)
	  snake_set_new_direction(GOING_RIGHT);
}

void game_anim()
{
	static uint32_t schedulerTimestamp_updateName = 0;
	if( HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET )
	{
		currentAnimation++;
		if(currentAnimation == MAX_ANIM_ID)
			currentAnimation = 0;

		while(HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == GPIO_PIN_RESET)
		{
			screen_show_letter((currentAnimation < 10)?(ASCII_ZERO + currentAnimation):(ASCII_UPPER_A + currentAnimation - 10));
			HAL_Delay(100);
		}
	}
	else if( HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET )
	{
		if(currentAnimation == 0)
			currentAnimation = MAX_ANIM_ID-1;
		else
			currentAnimation--;

		while(HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == GPIO_PIN_RESET)
		{
			screen_show_letter((currentAnimation < 10)?(ASCII_ZERO + currentAnimation):(ASCII_UPPER_A + currentAnimation - 10));
			HAL_Delay(100);
		}
	}

	switch(currentAnimation)
	{
		 case 0:
			 anim_random_run();
			 break;
		 case 1:
			 if(HAL_GetTick() - schedulerTimestamp_updateName > DELAY_PER_LETTER)
			 {
				 schedulerTimestamp_updateName = HAL_GetTick();
				 anim_name_word("DEFCON32! ");
			 }
			 break;
		 case 2:
			 anim_horizontal_run();
			 break;
		 case 3:
			 anim_vertical_run();
			 break;
		 case 4:
			 anim_diagonal_run();
			 break;
		 case 5:
			 anim_rotate_run();
			 break;
		 case 6:
			 if(HAL_GetTick() - schedulerTimestamp_updateName > DELAY_PER_LETTER)
			 {
				 schedulerTimestamp_updateName = HAL_GetTick();
				 anim_name_word("RAWR! ^.^ ");
			 }
			 break;
		 case 7:
			 anim_swipeAll_run();
			 break;
		 case 8:
			 anim_ball_run();
			 break;
		 case 9:
			 anim_life_run();
			 break;
		 default:
			 screen_fill(); HAL_Delay(10);
			 screen_clear(); HAL_Delay(10);
			 break;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM16_Init();
  MX_ADC_Init();
  /* USER CODE BEGIN 2 */



  //Generate seed from floating ADC pin
  HAL_ADC_Start(&hadc);
  uint32_t seed = 0;
  uint32_t currentVal = 0;
  for(int i=0; i<10; i++)
  {
	  HAL_ADC_PollForConversion(&hadc, 0xFF);
	  currentVal = HAL_ADC_GetValue(&hadc);
	  seed = seed + currentVal;
	  HAL_Delay(2);
  }
  srand(seed);	//input seed into sRand

  //start timer for screen refresh
  HAL_TIM_Base_Start_IT(&htim16);

  // SETUP games modes
  uint8_t firstBoot = 1;
  anim_life_setup();
  if(HAL_GPIO_ReadPin(BTA_GPIO_Port, BTA_Pin) == GPIO_PIN_RESET)
	  GAME_MODE = 1;
  else if(HAL_GPIO_ReadPin(BTB_GPIO_Port, BTB_Pin) == GPIO_PIN_RESET)
	  GAME_MODE = 2;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t row,col = 0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	while(HAL_GPIO_ReadPin(BTB_GPIO_Port, BTB_Pin) == GPIO_PIN_RESET && GAME_MODE != MAIN_MENU_ID) //HOLD "B" to enter the main menu
//	{
//		//clear the screen
//		if(row==0 && col==0)
//			screen_clear();
//
//		screen_set_bit(row, col, 1);
//		col++;
//		if(col==8)
//		{
//			col=0;
//			row++;
//		}
//		if(row==8)
//		{
//			GAME_MODE = MAIN_MENU_ID;
//			firstBoot = 1;
//			screen_show_letter(new_game_select + ASCII_ONE);	//show the game ID between 1 and 3
//			row = 0;
//			col = 0;
//		}
//
//		HAL_Delay(20);
//	}
//	//main menu was not loaded, return to game
//	if(row!=0 || col != 0)
//	{
//		screen_clear();
//		row = 0;	//reset main-menu-loading variables
//		col = 0;
//	}

	switch(GAME_MODE)
	{
	  case GAME_SNAKE_ID:
		  if(firstBoot == 1)
		  {
			  game_startNewGame();
			  firstBoot = 0;	//de-assert flag
		  }
		  game_snake();
		  break;
	  case GAME_RACE_ID:
		  gameRacer_run();
		  break;
	  case GAME_ANIM_ID:
		  game_anim();
		  break;
	  /*case MAIN_MENU_ID:
		  main_menu();
		  break;*/
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
