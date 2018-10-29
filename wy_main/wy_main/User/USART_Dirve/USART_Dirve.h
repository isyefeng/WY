#ifndef __USART_DIRVE_H
#define __USART_DIRVE_H

#include "stm32f10x_usart.h"
#include "stm32f10x.h"

#define		GPIO_TX_PIN		GPIO_Pin_9
#define		GPIO_RX_PIN		GPIO_Pin_10
#define		GPIO_TX_RORT	GPIOA
#define		GPIO_RX_RORT	GPIOA

#define   GPIO_TX2_PIN	 	GPIO_Pin_2
#define   GPIO_RX2_PIN	  GPIO_Pin_3
#define   GPIO_TX2_PORT 	GPIOA
#define   GPIO_RX2_PORT		GPIOA



#define             BLT_USART_BAUD_RATE                       9600

#define             BLT_USARTx                                USART2
#define             BLT_USART_APBxClock_FUN                 RCC_APB1PeriphClockCmd
#define             BLT_USART_CLK                             RCC_APB1Periph_USART2
#define             BLT_USART_GPIO_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define             BLT_USART_GPIO_CLK                       (RCC_APB2Periph_GPIOA )     
#define             BLT_USART_TX_PORT                         GPIOA   
#define             BLT_USART_TX_PIN                          GPIO_Pin_2
#define             BLT_USART_RX_PORT                         GPIOA 
#define             BLT_USART_RX_PIN                          GPIO_Pin_3
#define             BLT_USART_IRQ                             USART2_IRQn
#define             BLT_USART_IRQHandler                         USART2_IRQHandler


void USART_Config(void);
void USART2_CONFIG(void);
void USART_Sendbyt(USART_TypeDef* USARTx,uint16_t Data);
void Usart_SendStr(USART_TypeDef* USARTx,unsigned char * Data);
void BLT_USART_Config(void);

#endif /*__USART_DIRVE_H*/
