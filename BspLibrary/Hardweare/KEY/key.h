#ifndef __KEY_H_
#define __KEY_H_
#include "sys.h"

#define KEY PCin(10)// PC10

#define KEY_ON  1
#define KEY_OFF 0

void KEY_Init(void);
uint8_t KEY_Scan(void);

#endif 

