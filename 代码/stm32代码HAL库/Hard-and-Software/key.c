#include "sys.h"

char KEY_Scan(char mode)
{	 
	static char key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(Key_Left==0||Key_Mid==0||Key_Right==0||Key1==0))
	{
		HAL_Delay(10);//去抖动 
		key_up=0;
		if(Key_Left==0)			return Key_Left_Press;
		else if(Key_Mid==0)		return Key_Mid_Press;
		else if(Key_Right==0)	return Key_Right_Press;
		else if(Key1==0)			return Key1_Press;

	}else if(Key_Left==1&&Key_Mid==1&&Key_Right==1&&Key1==1)
	{
		key_up=1;
	}
 	return 0;// 无按键按下
}

int Function_Mode(void)
{
	printf("t0.txt=\"no\"\xFF\xFF\xFF");
	char Enter_Key_Press=0;
	int num=1;
	int ccc=0;
	while(!Enter_Key_Press)
	{
		switch(KEY_Scan(0))
		{
			case Key_Right_Press:
				num++;
			if(num==17)
				{num=1;}
				break;
			case Key_Left_Press:
				num--;
				if(num==0)
				{num=16;}
				break;
			case Key_Mid_Press:
				Enter_Key_Press=1;
				printf("t0.txt=\"ok\"\xFF\xFF\xFF");
				break;
			case Key1_Press:
				num=5;
				Enter_Key_Press=1;
				printf("C0.val=%d\xFF\xFF\xFF",num);
				printf("t0.txt=\"ok\"\xFF\xFF\xFF");
				break;
		}
		ccc++;
		if (ccc==10000)
		{
			ccc=0;
			printf("C0.val=%d\xFF\xFF\xFF",num);
		}
		
	}
	HAL_Delay(500);
	return num;
}
