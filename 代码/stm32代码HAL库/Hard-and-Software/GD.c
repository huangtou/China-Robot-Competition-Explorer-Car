#include "sys.h"

int GD(int i)
{
	int a;
	switch(i)
	{
		case 1:a=GD1;break;
		case 2:a=GD2;break;		
		case 5:a=GD5;break;
		case 6:a=GD6;break;
		case 7:a=GD7;break;
		case 8:a=GD8;break;
//Çá´¥¿ª¹Ø
		case 9:a=Qc1;break;
//¼¤¹â	
		case 11:a=jg1;break;
		case 12:a=jg2;break;
		
		default:a=-1;break;
	}
	return a;
}
