#include "sys.h"

static volatile uint8_t ccd_hwt101_switched;
static volatile float ccd_hwt101_switch_speed;
static float ccd_hwt101_angle;
static int (*ccd_hwt101_switch_callback)(void);
static int (*ccd_hwt101_end_callback)(void);

static int ccd_to_hwt101_transition_callback(void)
{
	float current_target = car.Target;
	int switch_now = (ccd_hwt101_switch_callback == NULL) || ccd_hwt101_switch_callback();

	if(!switch_now)
	{
		return 0;
	}

	HWT101_Calibrate();
	int av = (short)((ccd_hwt101_angle * 32768) / 180);
	car.Target_Angle = ((short)(av + car.yaw_ca)) * 360 / 65536.0f;
	car.Line_Search = 0;
	car.line = 0;
	car.OUT = 0.0f;
	car.state = 2;
	car.enco = 1;
	car.enco_counter = 0;
	encoder[1] = 0;
	encoder[2] = 0;
	encoder[3] = 0;
	encoder[4] = 0;
	pid_F.output = 0;
	pid_A.output = 0;
	pid_A.error = 0;
	pid_A.lasterror = 0;
	pid_A.preerror = 0;
	pid_R.output = 0;
	car.HWT101_getOnce = 1;
	ccd_hwt101_switch_speed = current_target;
	car.Target = current_target;
	car.callBack = ccd_hwt101_end_callback;
	ccd_hwt101_switched = 1;

	return 0;
}

int median_of_7(int a, int b, int c, int d, int e, int f, int g)
{
    int arr[7] = {a, b, c, d, e, f, g};
    // 完整冒泡排序
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr[3];  // 返回排序后的中间值
}

int median_of_21(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
                int a11, int a12, int a13, int a14, int a15, int a16, int a17, int a18, int a19, int a20, int a21)
{
    int arr[21] = {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,
                   a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21};
    // 完整冒泡排序
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 20 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    return arr[10];  // 返回排序后的中间值（第11个元素，下标为10）
}

void HWT101_Calibrate(void)//标定此时的值
{
//	car.yaw_ca = median_of_21(
//    car.yaw, car.yaw_1, car.yaw_2, car.yaw_3, car.yaw_4, car.yaw_5, car.yaw_6,
//    car.yaw_7, car.yaw_8, car.yaw_9, car.yaw_10, car.yaw_11, car.yaw_12, car.yaw_13,
//    car.yaw_14, car.yaw_15, car.yaw_16, car.yaw_17, car.yaw_18, car.yaw_19, car.yaw_20
//);
	car.yaw_ca = median_of_7(car.yaw,car.yaw_1,car.yaw_2,car.yaw_3,car.yaw_4,car.yaw_5,car.yaw_6);
//	printf("TL0.val=%d\xFF\xFF\xFF",car.yaw_ca);
//	car.yaw_ca=car.yaw_cl;
}

void HWT101_go(float angle,int speed,int (*callBack)(void))			//int (*callBack)(void)函数指针，填函数名，传址，传给callBack
{
	int av=(short)((angle*32768)/180);

	car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;

	car.Line_Search =0;							//
	car.state   = 2;								//Timer-6
	car.Target  =speed;							//
	car.line=0;											//Timer-6
	car.callBack=callBack;					//Timer-6回调函数
	pid_F.output=0;									//
	pid_R.output=0;									//

	HAL_TIM_Base_Start_IT(&htim6);	//打开事务中断//HAL_TIM_Base_Start：开启定时器计数->溢出事件//_IT：使能中断功能//所以产生溢出中断->HAL库的底层中断服务程序ISR会自动调用回调函数“HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)”（在Timer.c中）

	while(car.state)
	{
//			printf("TS1.val=%d\xFF\xFF\xFF",(int)((car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-car.OUT)));
//			printf("TS2.val=%d\xFF\xFF\xFF",(int)pid_1.output);
//		  Delay_ms(100);
		printf("%f,%f,%f,%f,%f,%d\n",KalmanFilter1(&kfp,car.Angle),car.Angle,pid_A.output,pid_1.output,car.Target_Angle-car.Angle,velocity[1]);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.enco=0;
	car.Target=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}
void HWT101_back(float angle,int speed,int (*callBack)(void))
{
	int av=(short)((angle*32768)/180);
	car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;

	car.Direct=-1;
	car.Line_Search =0;
	car.state   = 2;
	car.Target  =speed;
		car.line=0;
	car.callBack=callBack;//回调函数
	pid_F.output=0;
	pid_R.output=0;

	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断

	while(car.state)
	{
//			printf("TS1.val=%d\xFF\xFF\xFF",(int)((car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-car.OUT)));
//			printf("TS2.val=%d\xFF\xFF\xFF",(int)pid_1.output);
//		  Delay_ms(100);
		printf("%f,%f,%f,%f,%f,%d\n",KalmanFilter1(&kfp,car.Angle),car.Angle,pid_A.output,pid_1.output,car.Target_Angle-car.Angle,velocity[1]);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.enco=0;
	car.Target=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void HWT101_go_rotate(float angle,int speed,int (*callBack)(void))
{
	int av=(short)((angle*32768)/180);
	car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;
	car.line=0;
	car.Line_Search =0;
	car.state       =3;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	pid_F.output=0;
	pid_A.output=0;
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
	while(car.state)
	{
		printf("%f,%f,%f,%f,%d\n",car.Angle,pid_A.output,pid_1.output,car.Target_Angle-car.Angle,velocity[1]);

	}
	printf("%f,%f,%f,%f,%d\n",car.Angle,pid_A.output,pid_1.output,car.Target_Angle-car.Angle,velocity[1]);
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.Target =0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
	velocity[1]=0;velocity[2]=0;velocity[3]=0;velocity[4]=0;
}

void go_enco(int dis,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=1;
	car.Target_dis=(int)(dis*2700/50);
	car.Direct=dir;
	car.Line_Search = Line_Search;
	car.state       =5;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	car.enco=1;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
	while(car.state)
	{
		printf("%f,%f,%f\n",pid_A.output,pid_1.output,car.Target_Angle-car.Angle);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void go(u32 num,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=1;
	car.ADnum=num;
	car.Direct=dir;
	car.Line_Search = Line_Search;
	car.state       =1;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	car.enco=1;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
	while(car.state)
	{
		printf("%f\n",car.Pout);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void go_time(int tim,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=1;
	car.time=tim;
	car.Direct=dir;
	car.Line_Search = Line_Search;
	car.state       =6;
	car.Target  =1.0f*speed;
	car.callBack=callBack;//回调函数
	car.enco=0;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;
	HAL_TIM_Base_Start_IT(&htim10);//打开定时中断
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
	while(car.state)
	{
			printf("%f,%f,%d,%f,%f,%d,%d\n",car.OUT,car.Target,(int)(car.OUT*car.Target),car.find,((car.Target-pid_A.output-pid_R.output+pid_F.output)*(1+car.OUT)),(int)pid_1.output,velocity[1]);
	}
	if (__HAL_TIM_GET_FLAG(&htim10, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim10, TIM_FLAG_UPDATE);	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	HAL_TIM_Base_Stop_IT(&htim10);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void go_GD(int id,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=1;
	car.gd_id=id;
	car.Direct=dir;
	car.Line_Search = Line_Search;
	car.state       =7;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	car.enco=0;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
	while(car.state)
	{
			printf("%f,%d\n",car.Pout,car.time);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

//tim:加速/减速的时间（并不是函数执行的时间）
//dir方向:  0:灰度前进  -1:CCD倒车  1:CCD前进
//Line_Search巡线模式:  -1:左巡线  0:中巡线  1:右巡线

//car.line是否巡线：1巡，0不巡
void go_acc(int tim,int speed_start,int speed_end,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=1;
/////配置函数运行模式部分/////
	int ict;
	car.Direct=dir;									//Timer.c
	car.Line_Search = Line_Search;	//move.c
	car.state       =8;							//Timer.c
	car.Target  =speed_start;
	car.callBack=callBack;					//回调函数
	car.enco=1;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;
/////配置函数运行模式部分*****
/////函数启用部分（重要）负责PID计算，回调函数判断，结束指令发出等（具体内容在Timer.c）/////
	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断
/////函数启用部分（重要）负责PID计算，回调函数判断，结束指令发出等（具体内容在Timer.c）*****

/////显示屏部分/////
	for(ict=0;ict<=tim;ict++)
	{
		if (car.state==0) break;
		car.Target = (speed_end-speed_start)*(ict*ict/(float)tim/tim)+speed_start;//二次曲线减速
		printf("%f,%f\n",car.Pout,car.Target);
		Delay_us(999);
	}
/////维持函数运行部分/////
	while(car.state)//由Timer.c相关函数关闭
	{
			printf("%f,%f,%f\n",pid_A.output,pid_1.output,car.Target_Angle-car.Angle);
	}
/////维持函数运行部分*****
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
/////彻底结束函数部分*****
}

void go_acc_hwt101(int tim, int speed_start, int speed_end, int8_t dir, float angle, int (*callBack)(void))
{
		int av=(short)((angle*32768)/180);
		car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;											//确定目标角度

    int ict;

    // 配置函数运行模式部分
    car.Direct = dir;                  // 设置方向																//获取灰度/CCD数据
		car.Line_Search =0;																														//找线
    car.state = 2;                     // 状态2表示陀螺仪控制模式									//
    car.Target = speed_start;          // 初始速度目标														//设定初始速度
		car.line=0;																																		//
		car.OUT=0.0f;                      // 关闭循线时清除上一段灰度/CCD留下的转向修正
    car.callBack = callBack;           // 回调函数
    car.enco = 1;                      // 启用编码器															//开启编码器

    // 重置PID输出
    pid_F.output = 0;
    pid_A.output = 0;
		pid_A.error = 0;
		pid_A.lasterror = 0;
		pid_A.preerror = 0;
    pid_R.output = 0;

    // 启用定时器中断,负责PID计算和状态管理
    HAL_TIM_Base_Start_IT(&htim6);

    // 二次曲线加减速过程
    for(ict = 0; ict <= tim; ict++)
    {
        if (car.state == 0) break;  // 如果状态被设置为0,提前退出

        // 二次曲线计算当前目标速度
        car.Target = (speed_end - speed_start) * (ict * ict / (float)tim / tim) + speed_start;

        // 打印调试信息:当前速度输出,目标速度,角度偏差
        printf("%f,%f,%f\n", car.Pout, car.Target, car.Target_Angle - car.Angle);

        Delay_us(999);
    }

    // 维持函数运行,直到状态被改变
    while(car.state)
    {
        // 打印PID输出和角度偏差信息
        printf("%f,%f,%f\n", pid_A.output, pid_R.output, car.Target_Angle - car.Angle);
    }

    // 清理工作
    if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    }
    HAL_TIM_Base_Stop_IT(&htim6);

    // 停止运动
    move_control(0, 0, 0, 0);

    // 重置相关变量
    car.enco_counter = 0;
    car.Target = 0;
    car.enco = 0;
    car.Target_Angle = 0;
    encoder[1] = 0;encoder[2] = 0;encoder[3] = 0;encoder[4] = 0;
}

void go_acc_ccd_to_hwt101(int ccd_tim, int ccd_speed_start, int ccd_speed_end,
                          int hwt_tim, int hwt_speed_end, int8_t dir,
                          int8_t Line_Search, float angle,
                          int (*switchCallback)(void), int (*endCallback)(void))
{
	int ict;
	float hwt_speed_start;

	ccd_hwt101_switched = 0;
	ccd_hwt101_switch_speed = ccd_speed_start;
	ccd_hwt101_angle = angle;
	ccd_hwt101_switch_callback = switchCallback;
	ccd_hwt101_end_callback = endCallback;

	car.line = 1;
	car.OUT = 0.0f;
	car.Direct = dir;
	car.Line_Search = Line_Search;
	car.state = 8;
	car.Target = ccd_speed_start;
	car.callBack = ccd_to_hwt101_transition_callback;
	car.enco = 1;
	pid_F.output = 0;
	pid_A.output = 0;
	pid_R.output = 0;

	HAL_TIM_Base_Start_IT(&htim6);

	if(ccd_tim <= 0)
	{
		car.Target = ccd_speed_end;
	}
	else
	{
		for(ict = 0; ict <= ccd_tim; ict++)
		{
			if(car.state == 0 || ccd_hwt101_switched) break;
			car.Target = (ccd_speed_end - ccd_speed_start) *
			             (ict * ict / (float)ccd_tim / ccd_tim) + ccd_speed_start;
			Delay_us(999);
		}
	}

	while(car.state && !ccd_hwt101_switched)
	{
		/* 等待CCD阶段的切换条件，TIM6保持运行。 */
	}

	if(car.state && ccd_hwt101_switched)
	{
		hwt_speed_start = ccd_hwt101_switch_speed;
		if(hwt_tim <= 0)
		{
			car.Target = hwt_speed_end;
		}
		else
		{
			for(ict = 0; ict <= hwt_tim; ict++)
			{
				if(car.state == 0) break;
				car.Target = (hwt_speed_end - hwt_speed_start) *
				             (ict * ict / (float)hwt_tim / hwt_tim) + hwt_speed_start;
				Delay_us(999);
			}
		}
	}

	while(car.state)
	{
		/* 等待陀螺仪阶段的结束条件。 */
	}

	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)
	{
		__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter = 0;
	car.Target = 0;
	car.enco = 0;
	car.callBack = NULL;
	encoder[1] = 0;
	encoder[2] = 0;
	encoder[3] = 0;
	encoder[4] = 0;
	ccd_hwt101_switch_callback = NULL;
	ccd_hwt101_end_callback = NULL;
}

void enco_acc(int dis,int speed_start,int speed_end,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.Target_dis=(int)(dis*2700/50);
	car.Target = speed_start;
	car.Direct = dir;
	car.Line_Search = Line_Search;
	car.callBack = callBack;
	car.line = 1;
	car.state = 8;

	car.enco=0;
	pid_F.output=0;
	pid_A.output=0;
	pid_R.output=0;

	HAL_TIM_Base_Start_IT(&htim6);

	while ((car.Target_dis-car.enco_counter<=1) && (car.state != 0))
	{
		int bili = ((float)car.enco_counter) / car.Target_dis;
		car.Target = speed_start + (speed_end - speed_start)*bili*bili;

		if(abs(speed_start) < abs(speed_end))		//加速
		{
			if(car.Target<0)
			{
				car.Target = ((car.Target < speed_end) ? speed_end : ((car.Target > speed_start) ? speed_start : car.Target));
			}
			else
			{
				car.Target = ((car.Target < speed_start) ? speed_start : ((car.Target > speed_end) ? speed_end : car.Target));
			}
		}
		if(abs(speed_start) > abs(speed_end))		//减速
		{
			if(car.Target<0)
			{
				car.Target = ((car.Target < speed_start) ? speed_start : ((car.Target > speed_end) ? speed_end : car.Target));
			}
			else
			{
				car.Target = ((car.Target < speed_end) ? speed_end : ((car.Target > speed_start) ? speed_start : car.Target));
			}
		}

		Delay_us(999);
  }

	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.Target=0;
	car.enco=0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void HWT101_GD(float angle,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=0;
	int av=(short)((angle*32768)/180);

	car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;
	car.Target_Angle_1=car.Target_Angle;
	car.Line_Search =0;
	car.state       = 9;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	pid_F.output=0;
	pid_R.output=0;

	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断

	while(car.state)
	{
		printf("L0.val=%d\xFF\xFF\xFF",(int)car.Target);
		printf("TS1.val=%d\xFF\xFF\xFF",velocity[1]);
		printf("TS2.val=%d\xFF\xFF\xFF",velocity[2]);
		printf("TS3.val=%d\xFF\xFF\xFF",velocity[3]);
		printf("TS4.val=%d\xFF\xFF\xFF",velocity[4]);
//		printf("TS1.val=%d\xFF\xFF\xFF",(int)((car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-car.OUT)));
//		printf("TS2.val=%d\xFF\xFF\xFF",(int)((car.Target+pid_A.output+pid_R.output+pid_F.output)*(1.0f+car.OUT)));
		Delay_us(50);
//				printf("%f,%f,%f\n",pid_A.output,pid_1.output,car.Target_Angle-car.Angle);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.enco=0;
	car.Target  =0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

void HWT101_GD_QQB(float angle,int speed,int8_t dir,int8_t Line_Search,int (*callBack)(void))
{
	car.line=0;
	int av=(short)((angle*32768)/180);

	car.Target_Angle = ((short)(av+car.yaw_ca))*360/65536.0f;
	car.Target_Angle_1=car.Target_Angle;
	car.Line_Search =0;
	car.state       = 10;
	car.Target  =speed;
	car.callBack=callBack;//回调函数
	pid_F.output=0;
	pid_R.output=0;

	HAL_TIM_Base_Start_IT(&htim6);//打开事务中断

	while(car.state)
	{
		printf("L0.val=%d\xFF\xFF\xFF",(int)car.Target);
		printf("TS1.val=%d\xFF\xFF\xFF",velocity[1]);
		printf("TS2.val=%d\xFF\xFF\xFF",velocity[2]);
		printf("TS3.val=%d\xFF\xFF\xFF",velocity[3]);
		printf("TS4.val=%d\xFF\xFF\xFF",velocity[4]);
//		printf("TS1.val=%d\xFF\xFF\xFF",(int)((car.Target-pid_A.output-pid_R.output+pid_F.output)*(1.0f-car.OUT)));
//		printf("TS2.val=%d\xFF\xFF\xFF",(int)((car.Target+pid_A.output+pid_R.output+pid_F.output)*(1.0f+car.OUT)));
		Delay_us(50);
//				printf("%f,%f,%f\n",pid_A.output,pid_1.output,car.Target_Angle-car.Angle);
	}
	if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)  {	__HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);	}
	HAL_TIM_Base_Stop_IT(&htim6);
	move_control(0,0,0,0);
	car.enco_counter=0;
	car.enco=0;
	car.Target  =0;
	encoder[1]=0;encoder[2]=0;encoder[3]=0;encoder[4]=0;
}

//void HWT101_GD_go(float angle,int speed0,int speed1,u8 Road_State,int (*callBack)(void))//用前面两个光电辅助
//{
//	int av=(short)((angle*32768)/180);//angle最好不要超过130
//	car.avjuedui = ((short)(av+car.yaw_ca))*360/65536.0f;
//	car.avjuedui_1=car.avjuedui;
// 	car.PID_State   = 0;//不用CCD循线
//	car.Direct      = Road_State;//方向！
//	car.Line_Search = 3;
//	car.state       = 7;
//
//	Speed.Left_R =Speed.Left_B  =Speed.Left_D  =speed0;
//	Speed.Right_R=Speed.Right_B =Speed.Right_D =speed1;
//
//	car.callBack=callBack;//回调函数
//
//	TIM_Cmd(TIM8,ENABLE);//打开事务中断
//	while(car.state)
//	{
//		LCD_ShowAllNum(0,77,Speed.Left_R,4,12);
//		LCD_ShowAllNum(0,88,Speed.Right_R,4,12);
//		LCD_ShowAllNum(0,99,car.avjuedui_1,4,12);
//		LCD_ShowAllNum(0,111,car.avjuedui,4,12);
//		LCD_ShowAllNum(0,122,short2float(car.yaw_raw),4,12);
//		POINT_COLOR = BLUE;
//		LCD_ShowAllNum(0,140,car.time,5,12);
//		POINT_COLOR = RED;
//	}
//	TIM_Cmd(TIM8,DISABLE);
//}

