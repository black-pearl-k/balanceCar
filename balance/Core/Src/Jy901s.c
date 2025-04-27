#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "jy901s.h"
#include "string.h"
#include "stdio.h"
#include "vofa.h"



uint8_t data[11];
uint8_t data2[80];
uint8_t rcecive_data[11];

float Roll;
float Pitch;
float Yaw;

float w_Roll;
float w_Pitch;
float w_Yaw;

float a_Roll;
float a_Pitch;
float a_Yaw;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart == &huart1)        //jy901s
  {
		if(data[1] == 0x51)
		{
			a_Roll = 1.0*((data[3]<<8)|data[2])/32768*16*9.8;
			a_Pitch = 1.0*((data[5]<<8)|data[4])/32768*16*9.8;
			a_Yaw = 1.0*((data[7]<<8)|data[6])/32768*16*9.8;
		}
		else if(data[1] == 0x52)
		{
			w_Roll = 1.0*((data[3]<<8)|data[2])/32768*2000;
			w_Pitch = 1.0*((data[5]<<8)|data[4])/32768*2000;
			w_Yaw = 1.0*((data[7]<<8)|data[6])/32768*2000;
		}
		else if(data[1] == 0x53)
		{
			Roll = 1.0*((data[3]<<8)|data[2])/32768*180;
			Pitch = 1.0*((data[5]<<8)|data[4])/32768*180;
			Yaw = 1.0*((data[7]<<8)|data[6])/32768*180;
			Yaw = Yaw > 180.0 ? Yaw - 360.0 : Yaw;
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, data, sizeof(data));
    __HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	}
	if (huart == &huart2)              //vofa
	{
		//read_data(data2);
		if(data2[2] == 1)
		{
			state = control;
			
			if(data2[0] == 2)        {Temp = 10;LED_ON;}  //上下
			else if(data2[0] == 1)   {Temp = -10;LED_OFF;}
			else {Temp = 0;} 
			
			if(data2[1] == 1)        {cnt = 1;}                      //左右
			else if(data2[1] == 2)   {cnt = -1;}
			else                     {cnt = 0;}
		
		}

		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, data2, sizeof(data2));
     __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	}
}




