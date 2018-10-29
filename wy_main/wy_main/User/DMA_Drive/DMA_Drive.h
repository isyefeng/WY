#ifndef __DMA_DRIVE_H
#define __DMA_DRIVE_H
#include "stm32f10x.h"

// 要发送的数据大小
#define BUFFER_SIZE    				 32

//串口传输数据大小						 
#define Usart_m_size					 5000

#define DMA_M2M_Channelx				 DMA1_Channel3
#define DMA_Usart1toM_Channelx	 DMA1_Channel4

// 外设寄存器地址
#define  USART_DR_ADDRESS        (USART1_BASE+0x04)

void DMA_M2M_Config(void);
void Usart_DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1, uint16_t BufferLength);

#endif /*__DMA_DRIVE_H*/
