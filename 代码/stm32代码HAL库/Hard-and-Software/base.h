#ifndef __base_H__
#define __base_H__
#include "sys.h"

int median_of_21(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
                int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21) ;
void HWT101_Calibrate(void);
void HWT101_go(float angle,int speed,int (*callBack)(void));
void HWT101_back(float angle,int speed,int (*callBack)(void));
void HWT101_go_rotate(float angle,int speed,int (*callBack)(void));
void go_enco(int dis,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void go(u32 num,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void go_time(int tim,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void go_GD(int id,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void go_acc(int tim,int speed_start,int speed_end,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void go_acc_hwt101(int tim, int speed_start, int speed_end, int8_t dir, float angle, int (*callBack)(void));
void go_acc_ccd_to_hwt101(int ccd_tim, int ccd_speed_start, int ccd_speed_end,
                          int hwt_tim, int hwt_speed_end, int8_t dir,
                          int8_t Line_Search, float angle,
                          int (*switchCallback)(void), int (*endCallback)(void));
void enco_acc(int dis,int speed_start,int speed_end,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void HWT101_GD(float angle,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));
void HWT101_GD_QQB(float angle,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void));

#endif
