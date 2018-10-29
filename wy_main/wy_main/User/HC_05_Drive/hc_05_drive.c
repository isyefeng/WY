#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "LED.h"
#include "key_drive.h"
#include "EXTI_Dirve.h"		
#include "BUZ_Dirve.h"
#include "Systick_Dirve.h"
#include "TIM_Dirve.h"
#include "USART_Dirve.h"
#include "DMA_Drive.h"
#include "hc_05_drive.h"
#include "OSP_fifo.h"


#define	AT_TEST_COM	"AT\r\n"
#define	AT_GET_BLUETOOTH_PROWORD_COM	"AT+PSWD?\r\n"

#define 	Bluetooth_buf_size 512
fifo_tcb_t 	Bluetooth_control_tcb;
uint8_t 	Bluetooth_buf[Bluetooth_buf_size];

/**
  * @brief  配置 蓝牙 USART 的 NVIC 中断
  * @param  无
  * @retval 无
  */
static void Bluetooth_USART_NVIC_Congiguration ( void )
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig ( BluetoothNVIC_PriorityGroup_x );

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = Bluetooth_USART_IRQ;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void Bluetooth_control_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(Bluetooth_CONTROL_RCC, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = Bluetooth_STATE_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Bluetooth_CONTROL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Bluetooth_EN_PIN;
	GPIO_Init(Bluetooth_CONTROL_PORT, &GPIO_InitStruct);
}

static void Bluetooth_USART_Config ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	/* config USART clock */
	Bluetooth_USART_APBxClock_FUN ( Bluetooth_USART_CLK, ENABLE );
	Bluetooth_USART_GPIO_APBxClock_FUN ( Bluetooth_USART_GPIO_CLK, ENABLE );
	
	/* USART GPIO config */
	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  Bluetooth_USART_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Bluetooth_USART_TX_PORT, &GPIO_InitStructure);  
  
	/* Configure USART Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = Bluetooth_USART_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(Bluetooth_USART_RX_PORT, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = Bluetooth_USART_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(Bluetooth_USARTX, &USART_InitStructure);
	
	
	/* 中断配置 */
	USART_ITConfig ( Bluetooth_USARTX, USART_IT_RXNE, ENABLE ); //使能串口接收中断 	

	Bluetooth_USART_NVIC_Congiguration ();
	
	
	USART_Cmd(Bluetooth_USARTX, ENABLE);
	
	
}

void Bluetooth_USART_INT_FUN ( void )
{	
	unsigned char buf;
	if(USART_GetITStatus(Bluetooth_USARTX,USART_IT_RXNE)!=RESET)
	{		
		buf = USART_ReceiveData(Bluetooth_USARTX);
		fifo_buffer_push( &Bluetooth_control_tcb , &buf , 1 );
	}	 
}

void Bluetooth_Init(void)
{
	Bluetooth_USART_Config();
	Bluetooth_control_GPIO_Config();
	AT_MODO_OFF;
	fifo_buffer_init( &Bluetooth_control_tcb, Bluetooth_buf_size , Bluetooth_buf);
}

void Bluetooth_Drive_read( unsigned char*buf , uint32_t size)
{
	uint16_t fifo_receive = 0;
	fifo_receive = fifo_buffer_get_size(&Bluetooth_control_tcb);
	if (fifo_receive >= size)
	{
		fifo_buffer_pop( &Bluetooth_control_tcb , buf , size);
	}
}

void Bluetooth_Drive_Write(unsigned char *buf)
{
	Usart_SendStr(Bluetooth_USARTX, buf);
}

uint8_t Bluetooth_AT_Set(void)
{
	char AT_read_buf[20];
	char AT_buf[5] ="OK\r\n";
	Usart_SendStr(Bluetooth_USARTX , "AT\r\n");
	Delay_ms(0xff);
	Bluetooth_Drive_read((unsigned char*)AT_read_buf, 2);
	if ( strcmp( AT_read_buf, AT_buf) != 0)
	{
		return 	AT_TEST_SET_ERR;
	}
	Usart_SendStr(Bluetooth_USARTX , AT_GET_BLUETOOTH_PROWORD_COM);

	return 	AT_SET_NO_ERR;
}

