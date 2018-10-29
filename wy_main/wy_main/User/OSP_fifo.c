/********************************************************************************************
*            珠海科荟电器有限公司
*            http://www.zhkh.com
* 模块名称:软件FIFO支持
* 创建时间:2010-07-26
* 所属公司 :科荟
* 文件名称:FIFO.C
* 创建人 :揭成
* 功能描述: 提供软件FIFO支持
*
*----------------------------------------------------------------
修改记录:

修改日期:

修改人:

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
*   @函数名称:初始化一个FIFO
*   @创建人:揭成
*   @输入:
*      	self  :FIFO控制TCB
*     	buffer_size :FIFO缓冲区大小
*      	buf_ptr:FIFO缓冲区
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
*    @函数名称:移动当前FIFO位置+1
*   @创建人:揭成
*   @输入:
*      self  :FIFO控制TCB
*      pos  :位置属性
*    @ output :
*        移动后位置
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
   @函数名称:将大小为SIZE 的数据装入FIFO
   @创建人:揭成
   @输入:self :FIFO控制TCB
     data :待存数据缓冲
     size :数据大小
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
*  @函数名称:将大小为size数据移出FIFO
*    @创建人:揭成
*  @输入:
*     self  :FIFO控制TCB
*     data  :待存数据缓冲
*     size	:数据大小
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
*   @函数名称:得到当前FIFO中数据大小
*   @创建人:揭成
*   @输入:
*    self  :FIFO控制TCB
*   @ output :FIFO数据大小
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
*   @函数名称:清空FIFO
*   @创建人  :揭成
*   @输入    :self ,FIFO控制TCB
*   @输出    :FIFO数据大小
********************************************************************/
void fifo_buffer_empty( fifo_tcb_t * self )
{
	self->start_pos = 0;
	self->end_pos = 0;
	return;
}
