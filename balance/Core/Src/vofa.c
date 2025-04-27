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
    uint8_t data_Start_Num = 0; //��¼����λ��ʼ�ĵط�
    uint8_t data_End_Num = 0; //��¼����λ�����ĵط�
    uint8_t data_Num = 0; //��¼����λ��
    uint8_t minus_Flag = 0; //�ж��Ƿ�Ϊ����
    float data_return = 0; // �����õ�������
    for(i=0;i<200;i++) // ���ҵ��ںź͸�̾�ŵ�λ��
    {
        if(data[i] == '=') data_Start_Num = i + 1; // +1��ֱ�Ӷ�λ�����ݿ�ʼ�ĵط�
        if(data[i] == '!')
        {
            data_End_Num = i - 1;
            break;
        }
    }
    if(data[data_Start_Num] == '-') // ����Ǹ���
    {
        data_Start_Num += 1; // ����һλ������λ
        minus_Flag = 1; // ����flag
    }
    data_Num = data_End_Num - data_Start_Num + 1;
    if(data_Num == 4) //���ݹ�3λ
		{
        data_return = (data[data_Start_Num]-48)  + (data[data_Start_Num+2]-48)*0.1f +(data[data_Start_Num+3]-48)*0.01f;
    }
    else if(data_Num == 5) //���ݹ�4λ
    {
        data_return = (data[data_Start_Num]-48)*10 + (data[data_Start_Num+1]-48) + (data[data_Start_Num+3]-48)*0.1f +(data[data_Start_Num+4]-48)*0.01f;
    }
    else if(data_Num == 6) //���ݹ�5λ
    {
        data_return = (data[data_Start_Num]-48)*100 + (data[data_Start_Num+1]-48)*10 + (data[data_Start_Num+2]-48) +(data[data_Start_Num+4]-48)*0.1f + (data[data_Start_Num+5]-48)*0.01f;
    }
    if(minus_Flag == 1)  data_return = -data_return;
    return data_return;
}

void read_data(uint8_t data[])
{
	if(data[0] == 's' && data[1] == 'l')             //���ٶȻ�
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
	else if(data[0] == 's' && data[1] == 'r')        //���ٶȻ�
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
	else if(data[0] == 't')                         //ת��
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
			truck_result = analysis_data();                     //ת��Ŀ��Ƕ�
		}
		else                                         //Ŀ���ٶ�
		{
			L_target = analysis_data();
			R_target = analysis_data();
			L_target = limit(L_target ,0,20);
			R_target = limit(R_target ,0,20);
		}
	}
	else if(data[0] == 'u')                         //ֱ����
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
	else if(data[0] == 's' && data[1] == 't' && data[2] == 'o' && data[3] == 'p')   //����
	{
		Key = 0;
	}
	else if(data[0] == 'g' && data[1] == 'o')        //ͣ��
	{
		Key = 1;
	}
}


