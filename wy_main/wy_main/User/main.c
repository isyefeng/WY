#include "Includes.h"

#define		UCOS_TASK_START_PRIO	4
#define   	UCOS_TASK_START_STK_SIZE	128
OS_STK		UCOS_START_TASK_STK[UCOS_TASK_START_STK_SIZE];

#define		UCOS_TASK_LED_G_PRIO	5
#define   	UCOS_TASK_LED_G_STK_SIZE	128
OS_STK		UCOS_LED_G_TASK_STK[UCOS_TASK_LED_G_STK_SIZE];

#define		UCOS_TASK_temperture_PRIO	6
#define   	UCOS_TASK_temperture_STK_SIZE	128
OS_STK		UCOS_temperture_TASK_STK[UCOS_TASK_temperture_STK_SIZE];

#define		UCOS_TASK_MOTOR_PRIO	7
#define   	UCOS_TASK_MOTOR_STK_SIZE	128
OS_STK		UCOS_MOTOR_TASK_STK[UCOS_TASK_MOTOR_STK_SIZE];

#define		UCOS_TASK_USART_PRIO	8
#define  	 	UCOS_TASK_USART_STK_SIZE	128
OS_STK		UCOS_USART_TASK_STK[UCOS_TASK_USART_STK_SIZE];

void _UCOS_START_TASK(void *padat);
void _UCOS_LED_G_TASK(void *pdata);
void _UCOS_temperture_TASK(void *pdata);
void _UCOS_USART_TASK(void *pdata);

uint32_t time;
char receive_buf[100];

unsigned char _usart_fifo_buf[512] = {0,};
fifo_tcb_t  _usart_fifo_tcb;

float		 g_temperture_num = 0;	//温度
uint8_t 	 g_moto_open = 0;

void Delay(uint32_t cnt)
{
	while(cnt--);
}

void global_drive_init(void)
{
	LED_G_Config();
	LED_R_Config();
	USART_Config();
	Key1_Init();
	Key2_Init();
	ADVANCE_TIM_Init();
	fifo_buffer_init(&_usart_fifo_tcb, 512, _usart_fifo_buf);	//初始化FIFO
}

int main(void)
{
	delay_init();					
	global_drive_init();
	OSInit();
	OSTaskCreate(_UCOS_START_TASK, 0, &UCOS_START_TASK_STK[UCOS_TASK_START_STK_SIZE - 1], UCOS_TASK_START_PRIO);
	OSStart();
}

void _UCOS_START_TASK(void *padat)
{
	OS_CPU_SR cpu_sr=0;
//	OSStatInit(); //开启统计任务
	OS_ENTER_CRITICAL(); //进入临界区(关闭中断)
	OSTaskCreate(_UCOS_LED_G_TASK, 0, &UCOS_LED_G_TASK_STK[UCOS_TASK_LED_G_STK_SIZE - 1], UCOS_TASK_LED_G_PRIO);
	OSTaskCreate(_UCOS_temperture_TASK, 0, &UCOS_temperture_TASK_STK[UCOS_TASK_temperture_STK_SIZE - 1], UCOS_TASK_temperture_PRIO);
	OSTaskCreate(_MOTOR_TASK, 0, &UCOS_MOTOR_TASK_STK[UCOS_TASK_MOTOR_STK_SIZE - 1], UCOS_TASK_MOTOR_PRIO);
	OSTaskCreate(_UCOS_USART_TASK, 0, &UCOS_USART_TASK_STK[UCOS_TASK_USART_STK_SIZE - 1], UCOS_TASK_USART_PRIO);
	OSTaskSuspend(UCOS_TASK_START_PRIO);
	OS_EXIT_CRITICAL(); //退出临界区(开中断)
}

void _UCOS_LED_G_TASK(void *pdata)
{
	while(1)
	{
		LED_G_control(1);
		malls(1);
		LED_G_control(0);
		malls(1);
	}
}

void _UCOS_temperture_TASK(void *pdata)
{
	while(1)
	{
		mallms(100);
		g_temperture_num = DS_read_int_temperture();
	}
}

void _UCOS_USART_TASK(void *pdata)
{
	uint32_t index = 0;
	uint8_t buf[6] = {0,};

	while(1)
	{
		mallms(10);
		index++;
		if(index == 10)
		{
			printf("TEMPERTURE:");
			printf("%d",(int)g_temperture_num);
			printf(".%d\n",(int)((float)g_temperture_num*10)%10);
			index = 0;
		}
		Usart_Drive_read(buf, 1);
		if( buf[0] == 'A')
		{
			Usart_Drive_read(buf, 2);
			g_temperture_set = atoi((char*)buf);
			Moto2_rotate_angle(90);
//			Moto1_rotate_angle(90);
		}
		if(buf[0] == 'B')
		{
			memset(buf,0,6);
			Usart_Drive_read(buf, 1);
			g_flow_set = atoi((char*)buf);
			g_moto_open = 1;
			Moto2_rotate_angle(-90);
			Moto1_rotate_angle(-90);
		}
		if(buf[0] == 'C')
		{
			Usart_Drive_read(buf, 2);
			g_temperture_num = atoi((char*)buf);
		}
	}
}

