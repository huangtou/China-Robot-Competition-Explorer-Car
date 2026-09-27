#ifndef __key_H__
#define __key_H__
#include "sys.h"


#define Key_Left PDin(8)
#define Key_Mid PDin(9)
#define Key_Right PDin(10)
#define Key1 PDin(11)

#define Key_Right_Press 3
#define Key_Mid_Press 4
#define Key_Left_Press 5
#define Key1_Press 6

char KEY_Scan(char mode);
int Function_Mode(void);



#endif
