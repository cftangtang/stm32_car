#ifndef __USART_H_
#define __USART_H_
/*********************ͷ�ļ�����************************/

#include "stm32f10x.h"
#include <stdio.h>

/*********************��������**************************/

void usart1_init(u32 baudRate,uint8_t pre,uint8_t sub);

#endif
