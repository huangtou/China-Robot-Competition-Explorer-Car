#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#include "sys.h"
#define TIME_ZL 1800		//播报时长
#define TIME_READ 3000	//

int my_gd1(void);
int my_gd2(void);
int my_gd2_enco(void);
int my_gd5(void);
int my_gd7(void);
int my_gd8(void);
int my_gd2_gd8(void);
int my_gd56(void);
int my_gd17(void);
int my_qc1(void);//微动开关1
int my_jg1(void);
int my_jg1_gd2(void);
int my_jg2(void);
int my_jg2_gd8(void);
int my_jg2_gd2(void);
int my_jg1_jg2_gd2(void);
int my_enco(void);//指定距离
int my_enco_huidu(void);
int my_enco_huidu01(void);
int my_enco_gd2(void);
int my_enco_gd8(void);
int my_enco_gd2_gd8(void);
int my_time(void);//定时
int my_time_ZL(void);
int my_time_gd2(void);//定时
int my_time_qc1(void);
int my_speed_260(void);
int my_no_huidu(void);
int my_huidu(void);
int my_jiaozhun(void);
int my_jiaozhun56(void);
int my_huidu01(void);
int my_huidu56(void);
int my_huidu1011(void);
int my_huidu910(void);
int my_huidu01_1011(void);
int my_turn90(void);

void jiaozhun(void);
void begin(void);
void stop(int time);
void stop_read(int time);
void down_home(void);
void to_LBridge(void);
void up_LBridge(void);
void go_LBridge(void);
void down_LBridge(void);
void up_platform(void);
void turn_180(void);
void down_platform(void);
void ting_wen(void);//停稳
void ting_wen_d(void);//倒车停稳
void qi_bu(void);//起步
void qi_bu_d(void);//倒车起步
void HWT101_enco(int enco,int speed);

void Ready(void);
void LBridge(void);
void LBto2(void);
void to2toZL(void);
void toZLto4(void);
void JT1(void);
void JT1_1(void);
void to5(void);
void to5_test(void);
void toZLtoD(void);
void toJTtoZJ(void);
void toGQtoJTtoD(void);
void toJSDtomid(void);
void tobig(void);
void tohui(void);
void NO_QQB(void);
void QQB(void);
void JT2(void);
void toZLto3(void);
void gohome(void);

void flagroad_0(void);
void flagroad_1(void);
void flagroad_2(void);
void flagroad_3(void);
void flagroad_4(void);

void flagroad_back1(void);
void flagroad_back2(void);
void flagroad_back3(void);
void flagroad_back4(void);

void part1_pro(void);
void part2_pro(void);
void part3_pro(void);
void part4_pro(void);
#endif

