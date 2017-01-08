/****************************************************************************
* �ļ���   : uart.c
* �ļ����� �����ð��ش��ڲ����ʡ��ж����ȼ����жϷ����
* ����     ��zhangpeng
* ����ʱ�� ��2017.01.07
* ��Ŀ���� ���Զ���ʻ��
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
* ����ʱ�� :2017.01.07
* ����ֵ   :void
* ����     :
@ u32 baudRate:������
@ uint8_t pre :��ռ���ȼ�
@ uint8_t sub :��Ӧ���ȼ�
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
	USART_InitSturcture.USART_WordLength = USART_WordLength_8b;    //���ݿ��8λ
	USART_InitSturcture.USART_StopBits = USART_StopBits_1;         //һ��ֹͣλ
	USART_InitSturcture.USART_Parity = USART_Parity_No;            //����żУ��
	USART_InitSturcture.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturcture.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��������
	USART_Init(USART1 , &USART_InitSturcture);               
	
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
* ����ʱ�� :2017.01.07
* ����ֵ   :ch
* ����     :
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
* �������� :USART1_IRQHandler
* �������� :����1���жϷ�����
* ����     :zhangpeng
* ����ʱ�� :2017.01.07
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
