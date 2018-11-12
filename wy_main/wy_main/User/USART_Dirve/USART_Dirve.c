#include "Includes.h"

extern fifo_tcb_t  _usart_fifo_tcb;

uint8_t g_temperture_set = 35;
uint8_t g_flow_set = 0;

void NVIC_USART_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}

void NVIC_USART2_CONFIG(void)
{
	NVIC_InitTypeDef NVIC_InitSturct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitSturct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitSturct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitSturct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitSturct.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitSturct);
}

void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_TX_PIN;									//TX配置推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIO_TX_RORT,&GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//RX配置浮空输入
	GPIO_Init(GPIO_RX_RORT,&GPIO_InitStruct);	
}

void USART2_GPIO_CONFIG(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_TX2_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_TX2_PORT , &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_RX2_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_RX2_PORT , &GPIO_InitStruct);
}

void USART2_CONFIG(void)
{
	USART_InitTypeDef USART_InitStruct;

	USART2_GPIO_CONFIG();
	NVIC_USART2_CONFIG();
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART2 , &USART_InitStruct);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能中断标志位
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStruct;
	
	GPIO_config();
	NVIC_USART_Config();
	
	
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStruct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//使能中断标志位
	USART_Cmd(USART1, ENABLE);
}

void USART_Sendbyt(USART_TypeDef* USARTx,uint16_t Data)
{
	 USART_SendData(USARTx, Data);			//发送一个字节
	 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendStr(USART_TypeDef* USARTx,unsigned char *Data)
{
	while(*Data!='\0')
	{
		USART_Sendbyt(USARTx,*Data);
		Data++;
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	}
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

void BLT_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	BLT_USART_APBxClock_FUN(BLT_USART_CLK, ENABLE); 
	BLT_USART_GPIO_APBxClock_FUN(BLT_USART_GPIO_CLK, ENABLE);

	/* USART2 GPIO config */
	 /* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = BLT_USART_TX_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(BLT_USART_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = BLT_USART_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(BLT_USART_RX_PORT, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = BLT_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(BLT_USARTx, &USART_InitStructure); 
	
	/*	配置中断优先级 */
	NVIC_USART2_CONFIG();
	/* 使能串口2接收中断 */
	USART_ITConfig(BLT_USARTx, USART_IT_RXNE, ENABLE);

	USART_Cmd(BLT_USARTx, ENABLE);
	USART_ClearFlag(BLT_USARTx, USART_FLAG_TC);
}

void USART1_IRQHandler(void)
{
	unsigned char ucTemp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USART1);
 		fifo_buffer_push( &_usart_fifo_tcb , &ucTemp , 1 );	
	}	 
}

void Usart_Drive_read( unsigned char*buf , uint32_t size)
{
	uint16_t fifo_receive = 0;
	fifo_receive = fifo_buffer_get_size(&_usart_fifo_tcb);
	if (fifo_receive >= size)
	{
		fifo_buffer_pop( &_usart_fifo_tcb , buf , size);
	}
}


