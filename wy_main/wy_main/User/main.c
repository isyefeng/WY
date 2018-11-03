#include "Includes.h"

#define	malls(a)  	OSTimeDlyHMSM(0, 0, a, 0);
#define	mallms(a)	OSTimeDlyHMSM(0, 0, 0, a);

#define		UCOS_TASK_START_PRIO	4
#define   UCOS_TASK_START_STK_SIZE	128
OS_STK		UCOS_START_TASK_STK[UCOS_TASK_START_STK_SIZE];

#define		UCOS_TASK_LED_G_PRIO	5
#define   UCOS_TASK_LED_G_STK_SIZE	128
OS_STK		UCOS_LED_G_TASK_STK[UCOS_TASK_LED_G_STK_SIZE];

#define		UCOS_TASK_temperture_PRIO	6
#define   UCOS_TASK_temperture_STK_SIZE	128
OS_STK		UCOS_temperture_TASK_STK[UCOS_TASK_temperture_STK_SIZE];

#define		UCOS_TASK_MOTOR_PRIO	7
#define   UCOS_TASK_MOTOR_STK_SIZE	128
OS_STK		UCOS_MOTOR_TASK_STK[UCOS_TASK_MOTOR_STK_SIZE];

#define		UCOS_TASK_USART_PRIO	8
#define  	 	UCOS_TASK_USART_STK_SIZE	128
OS_STK		UCOS_USART_TASK_STK[UCOS_TASK_USART_STK_SIZE];

float g_temperture_num = 0;

void _UCOS_START_TASK(void *padat);
void _UCOS_LED_G_TASK(void *pdata);
void _UCOS_temperture_TASK(void *pdata);
void _MOTOR_TASK(void *pdata);
void _UCOS_USART_TASK(void *pdata);

uint32_t time;
char receive_buf[100];

void Delay(uint32_t cnt)
{
	while(cnt--);
}

void global_drive_init(void)
{
	LED_G_Config();
	LED_R_Config();
	USART_Config();
	ADVANCE_TIM_Init();
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

void _MOTOR_TASK(void *pdata)
{
	while(1)
	{
		mallms(10);
		Motor_switch();
		PWM_TUGO();
	}
}

void _UCOS_USART_TASK(void *pdata)
{
	while(1)
	{
		mallms(1000);
		Usart_SendStr(USART1, "TEMPERTURE:");
		printf("%f\n",g_temperture_num);
	}
}

