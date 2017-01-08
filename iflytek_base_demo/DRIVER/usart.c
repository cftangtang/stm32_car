/****************************************************************************
* �ļ���   : uart.c
* �ļ����� �����ð��ش��ڲ����ʡ��ж����ȼ����жϷ����
* ����     ��zhangpeng
* ����ʱ�� ��2016.12.29
* ��Ŀ���� ���ƶ������˵���
*---------------------------------------------------------------------#
* ��ʷ��¼
*   ����     ����    ����
*---------------------------------------------------------------------#

****************************************************************************/

#include "usart.h"

/****************************************************************************
* �������� :uart1_init
* �������� :��ʼ������1��TX->GPIOA9  RX->GPIOA10
* ����     :zhangpeng
* ����ʱ�� :2016.12.29
* ����ֵ   :void
* ����     :
@ u32 baudRate:������
@ uint8_t pre :��ռ���ȼ�
@ uint8_t sub :��Ӧ���ȼ�
****************************************************************************/
void usart1_init(u32 baudRate,uint8_t pre,uint8_t sub)
{
	USART_InitTypeDef USART_InitSturct;
	GPIO_InitTypeDef GPIO_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;              //��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA , &GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

	USART_InitSturct.USART_BaudRate = baudRate;
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;    //���ݿ��8λ
	USART_InitSturct.USART_StopBits = USART_StopBits_1;         //һ��ֹͣλ
	USART_InitSturct.USART_Parity = USART_Parity_No;            //����żУ��
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��������
	USART_Init(USART1 , &USART_InitSturct);               
	
	USART_ITConfig(USART1 , USART_IT_RXNE , ENABLE);            //ʹ�ܽ����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pre;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART1 , ENABLE);                                 //�򿪴���
	USART_ClearFlag(USART1 , USART_FLAG_TC);                    //�����һ�����ݷ���ʧ�ܵ�����
}
/****************************************************************************
* �������� :fputc
* �������� :�ض���c�⺯��printf��USART1
* ����     :zhangpeng
* ����ʱ�� :2016.12.29
* ����ֵ   :ch
* ����     :
@ int ch
@ FILE * f
****************************************************************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}
/****************************************************************************
* �������� :USART1_IRQHandler
* �������� :����1���жϷ�����
* ����     :zhangpeng
* ����ʱ�� :2016.12.29
* ����ֵ   :void
* ����     :void
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
