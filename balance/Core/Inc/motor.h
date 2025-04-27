#ifndef __MOTOR_H__
#define __MOTOR_H__


#define PWM_Max 1000

void Set_LSpeed(int PWM);
void Set_RSpeed(int PWM);
void Load(int LPWM,int RPWM);

#endif
