/********************************************************************************************
*            �麣�����������޹�˾
*            http://www.zhkh.com
* ģ������:���FIFO֧��
* ����ʱ��:2010-07-26
* ������˾ :����
* �ļ�����:FIFO.C
* ������ :�ҳ�
* ��������: �ṩ���FIFO֧��
*
*----------------------------------------------------------------
�޸ļ�¼:

�޸�����:

�޸���:

*********************************************************************************************************
*/
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

/*********************************************************************
*   @��������:��ʼ��һ��FIFO
*   @������:�ҳ�
*   @����:
*      	self  :FIFO����TCB
*     	buffer_size :FIFO��������С
*      	buf_ptr:FIFO������
*   @ output :NONE
*********************************************************************/
void  fifo_buffer_init( fifo_tcb_t*self, uint32_t buffer_size, unsigned char *buf_ptr )
{
	self->buffer_size = buffer_size;
	self->buffer_ptr  = buf_ptr ;
	self->end_pos     = 0;
	self->start_pos   = 0;
	return;
}
/*********************************************************************
*    @��������:�ƶ���ǰFIFOλ��+1
*   @������:�ҳ�
*   @����:
*      self  :FIFO����TCB
*      pos  :λ������
*    @ output :
*        �ƶ���λ��
*********************************************************************/
uint32_t  fifo_buffer_shift( fifo_tcb_t * self, uint32_t pos )
{
	if (( pos + 1 ) == self->buffer_size )
	{
		return ( 0 );
	}
	else
	{
		return ( pos + 1 );
	}
}
/*********************************************************************
   @��������:����СΪSIZE ������װ��FIFO
   @������:�ҳ�
   @����:self :FIFO����TCB
     data :�������ݻ���
     size :���ݴ�С
   @ output :NONE
*********************************************************************/
void fifo_buffer_push( fifo_tcb_t * self, unsigned char*data, uint32_t size )
{
	uint32_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		self->buffer_ptr[self->end_pos ] = data[cur_size];
		self->end_pos  = fifo_buffer_shift( self, self->end_pos );

		if ( self->end_pos == self->start_pos )
		{
			self->start_pos  = fifo_buffer_shift( self, self->start_pos );
		}
	}
	return;
}
/*********************************************************************
*  @��������:����СΪsize�����Ƴ�FIFO
*    @������:�ҳ�
*  @����:
*     self  :FIFO����TCB
*     data  :�������ݻ���
*     size	:���ݴ�С
*  @ output :NONE
*********************************************************************/
void fifo_buffer_pop( fifo_tcb_t * self, unsigned char*data, uint32_t size )
{
	uint32_t cur_size;
	for ( cur_size = 0;cur_size < size;cur_size++ )
	{
		data[cur_size] = self->buffer_ptr[self->start_pos];
		self->start_pos = fifo_buffer_shift( self, self->start_pos );
	}
	return ;
}
/********************************************************************
*   @��������:�õ���ǰFIFO�����ݴ�С
*   @������:�ҳ�
*   @����:
*    self  :FIFO����TCB
*   @ output :FIFO���ݴ�С
********************************************************************/
uint32_t fifo_buffer_get_size( fifo_tcb_t * self )
{
	if ( self->end_pos >= self->start_pos )
	{
		return self->end_pos - self->start_pos;
	}
	else
	{
		return self->buffer_size - ( self->start_pos - self->end_pos );
	}
}
/********************************************************************
*   @��������:���FIFO
*   @������  :�ҳ�
*   @����    :self ,FIFO����TCB
*   @���    :FIFO���ݴ�С
********************************************************************/
void fifo_buffer_empty( fifo_tcb_t * self )
{
	self->start_pos = 0;
	self->end_pos = 0;
	return;
}
