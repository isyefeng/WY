#ifndef __DMA_DRIVE_H
#define __DMA_DRIVE_H
#include "stm32f10x.h"

// Ҫ���͵����ݴ�С
#define BUFFER_SIZE    				 32

//���ڴ������ݴ�С						 
#define Usart_m_size					 5000

#define DMA_M2M_Channelx				 DMA1_Channel3
#define DMA_Usart1toM_Channelx	 DMA1_Channel4

// ����Ĵ�����ַ
#define  USART_DR_ADDRESS        (USART1_BASE+0x04)

void DMA_M2M_Config(void);
void Usart_DMA_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer,uint32_t* pBuffer1, uint16_t BufferLength);

#endif /*__DMA_DRIVE_H*/
