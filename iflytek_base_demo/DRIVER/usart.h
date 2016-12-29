#ifndef __USART_H_
#define __USART_H_
/*********************头文件声明************************/

#include "stm32f10x.h"
#include <stdio.h>

/*********************函数声明**************************/

void usart1_init(u32 baudRate,uint8_t pre,uint8_t sub);

#endif
