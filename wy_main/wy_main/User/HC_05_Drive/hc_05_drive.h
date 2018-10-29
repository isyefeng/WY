#ifndef __HC_05_DRIVE_H
#define __HC_05_DRIVE_H

#define   BT_USART_PORT  0

enum{
	AT_SET_NO_ERR = 0,
	AT_TEST_SET_ERR ,
};

/******************************** Bluetooth 连接引脚定义 ***********************************/
#if 		BT_USART_PORT 	>   		0
#define      Bluetooth_USART_BAUD_RATE                       115200
#define      Bluetooth_USARTX                                USART3
#define      Bluetooth_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      Bluetooth_USART_CLK                             RCC_APB1Periph_USART3
#define      Bluetooth_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      Bluetooth_USART_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define      Bluetooth_USART_TX_PORT                         GPIOB   
#define      Bluetooth_USART_TX_PIN                          GPIO_Pin_10
#define      Bluetooth_USART_RX_PORT                         GPIOB
#define      Bluetooth_USART_RX_PIN                          GPIO_Pin_11
#define      Bluetooth_USART_IRQ                             USART3_IRQn
#define      Bluetooth_USART_INT_FUN                         USART3_IRQHandler

#else

#define      Bluetooth_USART_BAUD_RATE                       115200
#define      Bluetooth_USARTX                                USART2
#define      Bluetooth_USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define      Bluetooth_USART_CLK                             RCC_APB1Periph_USART2
#define      Bluetooth_USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define      Bluetooth_USART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define      Bluetooth_USART_TX_PORT                         GPIOA  
#define      Bluetooth_USART_TX_PIN                          GPIO_Pin_2
#define      Bluetooth_USART_RX_PORT                         GPIOA
#define      Bluetooth_USART_RX_PIN                          GPIO_Pin_3
#define      Bluetooth_USART_IRQ                             USART2_IRQn
#define      Bluetooth_USART_INT_FUN                         USART2_IRQHandler

#endif

#define 	 Bluetooth_STATE_PIN				GPIO_Pin_6
#define	   Bluetooth_EN_PIN						GPIO_Pin_4
#define 	 Bluetooth_CONTROL_RCC			RCC_APB2Periph_GPIOA
#define	   Bluetooth_CONTROL_PORT			GPIOA


#define            BluetoothNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2

#define		 AT_MODO_ON  		GPIO_SetBits(Bluetooth_CONTROL_PORT, Bluetooth_EN_PIN)
#define		 AT_MODO_OFF		GPIO_ResetBits(Bluetooth_CONTROL_PORT, Bluetooth_EN_PIN)

void Bluetooth_USART_Config ( void );
void Bluetooth_Init(void);
void Bluetooth_Drive_read( unsigned char*buf , uint32_t size);
void Bluetooth_control_GPIO_Config(void);
uint8_t Bluetooth_AT_Set(void);

#endif


