#ifndef __move_H__
#define __move_H__
#include "sys.h"
typedef struct 
{
	float kp;
	float ki;
    float kd;
    float error;
    float lasterror;
	float preerror;
    float integral;
    float output;
	
}PID;

float mabs(float a);
void limit(float* integral,float limitation);

void move_init(void);
void move_control(int wheel_1,int wheel_2,int wheel_3,int wheel_4);
void move_velocity(void);
void move_encoder(void);

void PID_Init(volatile PID* pid, float kp, float ki, float kd);

void PID_calc_A(volatile PID*pid, float target, float actual);
void PID_calc_V(volatile PID*pid, float target, float actual);

void huidu_ccd_pid(void);

#endif
