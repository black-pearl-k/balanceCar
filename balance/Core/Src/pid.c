#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "jy901s.h"
#include "motor.h"
#include "pid.h"


float angle_Sum = 0;
float angle_result = 0;
float angle_last_err;
float angle_err = 0;

float truck_Sum = 0;
float truck_result = 0;
float truck_last_err = 0;

void PID_Init(PID *tt,float P,float I,float D,pid_type type1,data_type type2)
{
	tt->kp = P;
	tt->ki = I;
	tt->kd = D;
	tt->error = 0;
	tt->last_error = 0;
	tt->last_last_error = 0;
	tt->out = 0;
	tt->target = 0;
	tt->actuall = 0;
	tt->D_actuall = 0;
	tt->DD_actuall = 0;
	tt->type1 = type1;
	tt->type2 = type2;
}

float pid_figure(PID *tt)
{
	if(tt->type2 == discrete)           
	{
		if(tt->type1 == position)
		{
			tt->error = *(tt->target) - *(tt->actuall);
			tt->error_sum += tt->error;
			tt->out = tt->kp * tt->error
							+ tt->ki * tt->error_sum
							+ tt->kd * (tt->error - tt->last_error);
			tt->last_error = tt->error;
			return tt->out;
		}
		else
		{
			tt->error = *(tt->target) - *(tt->actuall);
			tt->out += tt->kp * (tt->error - tt->last_error)
							 + tt->ki * tt->error
							 + tt->kd * (tt->error - 2*tt->last_error + tt->last_last_error);
			tt->last_last_error = tt->last_error;
			tt->last_error = tt->error;
			return tt->out;
		}
	}
	else
	{
		if(tt->type1 == position)
		{
			tt->error = *(tt->target) - *(tt->actuall);
			tt->error_sum += tt->error;
			tt->out = tt->kp * tt->error
							+ tt->ki * tt->error_sum
							+ tt->kd * *(tt->D_actuall);
			return tt->out;
		}
		else
		{
			tt->error = *(tt->target) - *(tt->actuall);
			tt->out += tt->kp * *(tt->D_actuall)
							 + tt->ki * tt->error
							 + tt->kd * *(tt->DD_actuall);
			return tt->out;
		}
	}
}

void angle_pid(float L_S, float R_S, float Target)
{
    angle_err = 1.0*(L_S + R_S)/2 - Target;
    angle_Sum += angle_err;
		angle_Sum = limit(angle_Sum,-20,20);
		float Kp = 0.1;
    float Ki = 0.05;
		float Kd = 0.15;
    angle_result = Kp*angle_err + Ki*angle_Sum + Kd*(angle_err - angle_last_err); 
		angle_last_err = angle_err; 
}

void truck_pid(void)
{
	float err,Kp,Ki,Kd;                //1 2 3 4 5 6  7 8 9 10 11 12
	Kp = 1.5;
	Ki = 0;
	Kd = 0.04;
	if(IS_L6_DETECTED() || IS_L7_DETECTED()){err = 0;}
	
	if(IS_L5_DETECTED()) {err = 1;}
	if(IS_L8_DETECTED()) {err = -1;}
	
	if(IS_L4_DETECTED()) {err = 2;}
	if(IS_L9_DETECTED()) {err = -2;}
	
	if(IS_L3_DETECTED()) {err = 3;}
	if(IS_L10_DETECTED()) {err = -3;}
	
	if(IS_L2_DETECTED()) {err = 4;}
	if(IS_L11_DETECTED()) {err = -4;}

	
	truck_Sum += err;
	
	truck_result = Kp * err
	             + Ki * truck_Sum
				 + Kd * (err - truck_last_err);
	
	truck_last_err = err; 
}


