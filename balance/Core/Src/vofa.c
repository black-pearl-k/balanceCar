#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "jy901s.h"
#include "motor.h"
#include "pid.h"
#include "oled.h"
#include "font.h"
#include "stdio.h"
#include "string.h"
#include "vofa.h"


float analysis_data(void)
{
		uint8_t i=0;
    uint8_t data_Start_Num = 0; //记录数据位开始的地方
    uint8_t data_End_Num = 0; //记录数据位结束的地方
    uint8_t data_Num = 0; //记录数据位数
    uint8_t minus_Flag = 0; //判断是否为负数
    float data_return = 0; // 解析得到的数据
    for(i=0;i<200;i++) // 查找等于号和感叹号的位置
    {
        if(data[i] == '=') data_Start_Num = i + 1; // +1是直接定位到数据开始的地方
        if(data[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(data[data_Start_Num] == '-') // 如果是负数
    {
        data_Start_Num += 1; // 后移一位到数据位
        minus_Flag = 1; // 负数flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) //数据共3位
		{
        data_return = (data[data_Start_Num]-48)  + (data[data_Start_Num+2]-48)*0.1f +(data[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) //数据共4位
    {
        data_return = (data[data_Start_Num]-48)*10 + (data[data_Start_Num+1]-48) + (data[data_Start_Num+3]-48)*0.1f +(data[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) //数据共5位
    {
        data_return = (data[data_Start_Num]-48)*100 + (data[data_Start_Num+1]-48)*10 + (data[data_Start_Num+2]-48) +(data[data_Start_Num+4]-48)*0.1f + (data[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
    return data_return;
}

void read_data(uint8_t data[])
{
	if(data[0] == 's' && data[1] == 'l')             //左速度环
	{
		if(data[2] == 'P')
		{
			Lspeed.kp = analysis_data();
		}
		else if(data[2] == 'I')                        
		{
			Lspeed.ki = analysis_data();
		}
	}
	else if(data[0] == 's' && data[1] == 'r')        //右速度环
	{
		if(data[2] == 'P')
		{
			Rspeed.kp = analysis_data();
		}
		else if(data[2] == 'I')
		{
			Rspeed.ki = analysis_data();
		}
	}
	else if(data[0] == 't')                         //转向环
	{
		if(data[1] == 'P')
		{
			turn.kp = analysis_data();
		}
		else if(data[1] == 'D')
		{
			turn.kd = analysis_data();
		}
		else if(data[1] == 'T')
		{
			truck_result = analysis_data();                     //转向环目标角度
		}
		else                                         //目标速度
		{
			L_target = analysis_data();
			R_target = analysis_data();
			L_target = limit(L_target ,0,20);
			R_target = limit(R_target ,0,20);
		}
	}
	else if(data[0] == 'u')                         //直立环
	{
		if(data[1]=='P')
		{
			up.kp = analysis_data();
		}
		else if(data[1] == 'D')
		{
			up.kd = analysis_data();
		}
		else if(data[1] == 'I')
		{
			up.ki = analysis_data();
		}
	}
	else if(data[0] == 's' && data[1] == 't' && data[2] == 'o' && data[3] == 'p')   //发车
	{
		Key = 0;
	}
	else if(data[0] == 'g' && data[1] == 'o')        //停车
	{
		Key = 1;
	}
}


