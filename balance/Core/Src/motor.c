#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "jy901s.h"
#include "motor.h"

void Set_LSpeed(int PWM)
{
	PWM = PWM > PWM_Max ? PWM_Max : PWM < -PWM_Max ? -PWM_Max : PWM;
	if(PWM > 0)
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, PWM);
	}
	else
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, -PWM);
	}
}

void Set_RSpeed(int PWM)
{
	PWM = PWM > PWM_Max ? PWM_Max : PWM < -PWM_Max ? -PWM_Max : PWM;
	if(PWM > 0)
	{
		HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, PWM);
	}
	else
	{
		HAL_GPIO_WritePin(BIN1_GPIO_Port,BIN1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(BIN2_GPIO_Port,BIN2_Pin,GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, -PWM);
	}
}

void Load(int LPWM,int RPWM)
{
	Set_LSpeed(LPWM);
	Set_RSpeed(RPWM);
}
