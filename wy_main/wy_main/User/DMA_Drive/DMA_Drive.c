#include "DMA_Drive.h"
#include "stm32f10x.h"

const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
uint32_t		Buff[BUFFER_SIZE];
uint8_t			Buff1[Usart_m_size];

//  uint32_t DMA_PeripheralBaseAddr;   // �����ַ
//  uint32_t DMA_MemoryBaseAddr;       // �洢����ַ
//  uint32_t DMA_DIR;                  // ���䷽��
//  uint32_t DMA_BufferSize;           // ������Ŀ
//  uint32_t DMA_PeripheralInc;        // �����ַ����ģʽ
//  uint32_t DMA_MemoryInc;            // �洢����ַ����ģʽ
//  uint32_t DMA_PeripheralDataSize;   // �������ݿ��
//  uint32_t DMA_MemoryDataSize;       // �洢�����ݿ��
//  uint32_t DMA_Mode;                 // ģʽѡ��
//  uint32_t DMA_Priority;             // ͨ�����ȼ�
//  uint32_t DMA_M2M;                  // �洢�����洢��ģʽ

/**
  * @brief  M-M		DMA����
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
	DMA_InitStruct.DMA_BufferSize	= BUFFER_SIZE;				//�������ݳ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Word;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//һ��ѭ��ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
	
	DMA_Init(DMA_M2M_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_M2M_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC3);
}

/**
  * @brief  M-USART		DMA����
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
	DMA_InitStruct.DMA_BufferSize	= Usart_m_size;				//�������ݳ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//һ��ѭ��ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Usart1toM_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_Usart1toM_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/**
  * @brief  M-USART		DMA����
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
	DMA_InitStruct.DMA_BufferSize	= Usart_m_size;				//�������ݳ���
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_MemoryDataSize	=	DMA_MemoryDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;					//һ��ѭ��ģʽ
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA_Usart1toM_Channelx,&DMA_InitStruct);
	DMA_Cmd(DMA_Usart1toM_Channelx,ENABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
}

/**
  * @brief  �ַ����Ƚ�
  * @param    
  * @retval ��ͬ����1����ͬ����0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* ���ݳ��ȵݼ� */
  while(BufferLength--)
  {
    /* �ж���������Դ�Ƿ��Ӧ��� */
    if(*pBuffer != *pBuffer1)
    {
      /* ��Ӧ����Դ����������˳�������������0 */
      return 0;
    }
    /* ������������Դ�ĵ�ַָ�� */
    pBuffer++;
    pBuffer1++;
  }
  /* ����жϲ��Ҷ�Ӧ������� */
  return 1;  
}
