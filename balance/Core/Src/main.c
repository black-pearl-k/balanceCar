/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "jy901s.h"
#include "motor.h"
#include "pid.h"
#include "oled.h"
#include "font.h"
#include "stdio.h"
#include "string.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "vofa.h"
#include <stdlib.h>

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
short encoder_L = 0;
short encoder_R = 0;

float L_Speed = 0.0;
float R_Speed = 0.0;

float L_target = 0.0;
float R_target = 0.0;

PID Lspeed;
PID Rspeed;
PID Angle;
PID up;
PID turn;

car_state state;

char data_string[80];

int Key = 0;
float cnt = 35;
float Temp = 0;
int Temp2 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim6)
	{
		encoder_L = (short)__HAL_TIM_GET_COUNTER(&htim3);                
		encoder_R = -(short)__HAL_TIM_GET_COUNTER(&htim4);
		__HAL_TIM_SET_COUNTER(&htim3,0);
		__HAL_TIM_SET_COUNTER(&htim4,0);
		L_Speed = encoder_L;
		R_Speed = encoder_R;                                   

		mpu_dmp_get_data(&Ux,&Uy,&Uz);                         
		if(state == go_on)
		{
			angle_pid(L_Speed, R_Speed,15);
			L_target = pid_figure(&up) - pid_figure(&turn);
			R_target = pid_figure(&up) + pid_figure(&turn);
			Load(pid_figure(&Lspeed),pid_figure(&Rspeed));
		}
		else if(state == find_black)
		{
			angle_pid(L_Speed, R_Speed,20);
			truck_pid();
			L_target = pid_figure(&up) - truck_result;
			R_target = pid_figure(&up) + truck_result;
			Load(pid_figure(&Lspeed),pid_figure(&Rspeed));
		}
		else if(state == control)
		{
			if(Temp2 == 0)
			{
				Temp2++;
				cnt = 0;
			}
				
			angle_pid(L_Speed, R_Speed,Temp);
			L_target = pid_figure(&up) - cnt;
			R_target = pid_figure(&up) + cnt;
			Load(pid_figure(&Lspeed),pid_figure(&Rspeed));
		}
		
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	state = go_on;
	/*HAL_Delay(5);
	OLED_Init();
	sprintf(data_string,"hellow");
	OLED_PrintString(0,0,data_string,&font16x16,OLED_COLOR_NORMAL);*/
	
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, data, sizeof(data));
  __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data2, sizeof(data2));
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	
	PID_Init(&Lspeed,62.19,11.46,0,increment,discrete);
	PID_Init(&Rspeed,62.19,11.46,0,increment,discrete);
	Lspeed.actuall = &L_Speed;
	Lspeed.target = &L_target;
	Rspeed.actuall = &R_Speed;
	Rspeed.target = &R_target;
	
	PID_Init(&up,0.79,0.01,-0.62,position,discrete);
	up.actuall = &Uy;
	up.target = &angle_result;
	
	PID_Init(&turn,0.47,0,0.64,position,discrete);
	turn.actuall = &Uz;
	turn.target = &cnt;
	
	MPU_Init();
	mpu_dmp_init();
	
	HAL_TIM_Base_Start_IT(&htim6);
	
	//Load(500,500);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(!GET_LINE_SENSORS() && abs(Uz) <= 30 && state != control) 
		{
			state = go_on;
		}
		else if((GET_LINE_SENSORS() || abs(Uz) > 30) &&  state != control)
		{
			state = find_black;
		}
		

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
