#ifndef __PID_H__
#define __PID_H__

#define limit(num, Low, High) ((num) > (High) ? (High) : ((num) < (Low) ? (Low) : (num)))

typedef enum
{
	position = 0,    //post
	increment        //add
}pid_type;

typedef enum
{
	discrete = 0,    //li san
	continuous       //lian xu
}data_type;

typedef struct
{
	float error;
	float out;
	float last_error;
	float last_last_error;
	float *actuall;
	float *target;
	float *D_actuall;
	float *DD_actuall;
	float kp;
	float ki;
	float kd;
	float error_sum;
	pid_type type1;
	data_type type2;
}PID;

void PID_Init(PID *tt,float P,float I,float D,pid_type type1,data_type type2);
float pid_figure(PID *tt);
void angle_pid(float L_S, float R_S, float Target);
void truck_pid(void);

extern float angle_Sum;
extern float angle_result; 
extern float angle_err;

extern float truck_Sum;
extern float truck_result;
extern float truck_last_err;

#endif
