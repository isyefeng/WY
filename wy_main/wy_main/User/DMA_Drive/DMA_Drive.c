#include "DMA_Drive.h"
#include "stm32f10x.h"

const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
uint32_t		Buff[BUFFER_SIZE];
uint8_t			Buff1[Usart_m_size];

//  uint32_t DMA_PeripheralBaseAddr;   // 外设地址
//  uint32_t DMA_MemoryBaseAddr;       // 存储器地址
//  uint32_t DMA_DIR;                  // 传输方向
//  uint32_t DMA_BufferSize;           // 传输数目
//  uint32_t DMA_PeripheralInc;        // 外设地址增量模式
//  uint32_t DMA_MemoryInc;            // 存储器地址增量模式
//  uint32_t DMA_PeripheralDataSize;   // 外设数据宽度
//  uint32_t DMA_MemoryDataSize;       // 存储器数据宽度
//  uint32_t DMA_Mode;                 // 模式选择
//  uint32_t DMA_Priority;             // 通道优先级
//  uint32_t DMA_M2M;                  // 存储器到存储器模式

/**
  * @brief  M-M		DMA传输
  * @param    
  * @retval 
  */
void DMA_M2M_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)aSRC_Const_Buffer;
	DMA_InitStruct.DMA_MemoryBaseAddr	= (uint32_t)Buff;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC  ;
	DMA_InitStruct.DMA_BufferSize	= BUFFER_SIZE;				//传输数据长度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//一次循环模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	
	DMA_Init(DMA_M2M_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_M2M_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC3);
}

/**
  * @brief  M-USART		DMA传输
  * @param    
  * @retval 
  */
void Usart_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr =USART_DR_ADDRESS;
	DMA_InitStruct.DMA_MemoryBaseAddr	= (uint32_t)Buff1;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStruct.DMA_BufferSize	= Usart_m_size;				//传输数据长度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//一次循环模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Usart1toM_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_Usart1toM_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/**
  * @brief  M-USART		DMA传输
  * @param    
  * @retval 
  */
void Usart_to_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	DMA_InitStruct.DMA_PeripheralBaseAddr =USART_DR_ADDRESS;
	DMA_InitStruct.DMA_MemoryBaseAddr	= (uint32_t)Buff1;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize	= Usart_m_size;				//传输数据长度
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//一次循环模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Usart1toM_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_Usart1toM_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/**
  * @brief  字符串比较
  * @param    
  * @retval 相同返回1，不同返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}
