/****************************************************************************
* 文件名   : uart.c
* 文件功能 ：配置板载串口波特率、中断优先级、中断服务等
* 作者     ：zhangpeng
* 创建时间 ：2017.01.07
* 项目名称 ：自动驾驶仪
*---------------------------------------------------------------------#
* 历史纪录
*   日期     作者    描述
*---------------------------------------------------------------------#

****************************************************************************/

#include "usart.h"

/****************************************************************************
* 函数名称 :uart1_init
* 函数功能 :初始化串口1，TX->GPIOA9  RX->GPIOA10
* 作者     :zhangpeng
* 创建时间 :2017.01.07
* 返回值   :void
* 参数     :
@ u32 baudRate:波特率
@ uint8_t pre :抢占优先级
@ uint8_t sub :响应优先级
****************************************************************************/
void usart1_init(u32 baudRate,uint8_t pre,uint8_t sub)
{
	USART_InitTypeDef USART_InitSturcture;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	USART_InitSturcture.USART_BaudRate = baudRate;
	USART_InitSturcture.USART_WordLength = USART_WordLength_8b;    //数据宽度8位
	USART_InitSturcture.USART_StopBits = USART_StopBits_1;         //一个停止位
	USART_InitSturcture.USART_Parity = USART_Parity_No;            //无奇偶校验
	USART_InitSturcture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturcture.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART1 , &USART_InitSturcture);               
	
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);            //使能接收中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1 , ENABLE);                                 //打开串口
	USART_ClearFlag(USART1 , USART_FLAG_TC);                    //解决第一个数据发送失败的问题
}
/****************************************************************************
* 函数名称 :fputc
* 函数功能 :重定向c库函数printf到USART1
* 作者     :zhangpeng
* 创建时间 :2017.01.07
* 返回值   :ch
* 参数     :
@ int ch
@ FILE * f
****************************************************************************/
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) ch;      
	return ch;
}
/****************************************************************************
* 函数名称 :USART1_IRQHandler
* 函数功能 :串口1的中断服务函数
* 作者     :zhangpeng
* 创建时间 :2017.01.07
* 返回值   :void
* 参数     :void
****************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			ch = USART_ReceiveData(USART1);
			printf("%c",ch);			
	} 
}
