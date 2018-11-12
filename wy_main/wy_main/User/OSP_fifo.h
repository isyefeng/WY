#ifndef __OSP_FIFO_
#define __OSP_FIFO_

#include "Includes.h"

/********************************************************************/
//for FIFO
/********************************************************************/
typedef struct
{
	uint32_t 		  start_pos;
	uint32_t 		  end_pos;
	uint32_t		  buffer_size;
	unsigned char *buffer_ptr;
} fifo_tcb_t;



void  		fifo_buffer_init( fifo_tcb_t*self, uint32_t buffer_size, unsigned char *buf_ptr );
uint32_t  	fifo_buffer_shift( fifo_tcb_t * self, uint32_t pos );
void		fifo_buffer_push( fifo_tcb_t * self, unsigned char*data, uint32_t size );
void 		fifo_buffer_pop( fifo_tcb_t * self, unsigned char*data, uint32_t size );
uint32_t 	fifo_buffer_get_size( fifo_tcb_t * self );
void 		fifo_buffer_empty( fifo_tcb_t * self );

#endif
