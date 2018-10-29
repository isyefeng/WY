#include "Includes.h"

#define	malls(a)  	OSTimeDlyHMSM(0, 0, a, 0);
#define	mallms(a)	OSTimeDlyHMSM(0, 0, 0, a);

#define		UCOS_TASK_START_PRIO	4
#define   UCOS_TASK_START_STK_SIZE	128
OS_STK		UCOS_START_TASK_STK[UCOS_TASK_START_STK_SIZE];

#define		UCOS_TASK_LED_G_PRIO	5
#define   UCOS_TASK_LED_G_STK_SIZE	128
OS_STK		UCOS_LED_G_TASK_STK[UCOS_TASK_LED_G_STK_SIZE];

#define		UCOS_TASK_LED_R_PRIO	6
#define   UCOS_TASK_LED_R_STK_SIZE	128
OS_STK		UCOS_LED_R_TASK_STK[UCOS_TASK_LED_R_STK_SIZE];

#define		UCOS_TASK_MOTOR_PRIO	7
#define   UCOS_TASK_MOTOR_STK_SIZE	128
OS_STK		UCOS_MOTOR_TASK_STK[UCOS_TASK_MOTOR_STK_SIZE];

void _UCOS_START_TASK(void *padat);
void _UCOS_LED_G_TASK(void *pdata);
void _UCOS_LED_R_TASK(void *pdata);
void _MOTOR_TASK(void *pdata);

uint32_t time;
char receive_buf[100];

void Delay(uint32_t cnt)
{
	while(cnt--);
}

int main(void)
{
	delay_init();					
	LED_G_Config();
	LED_R_Config();
	OSInit();
	OSTaskCreate(_UCOS_START_TASK, 0, &UCOS_START_TASK_STK[UCOS_TASK_START_STK_SIZE - 1], UCOS_TASK_START_PRIO);
	OSStart();
	while(1)
	{	
		;
	}
}

void _UCOS_START_TASK(void *padat)
{
	OS_CPU_SR cpu_sr=0;
//	OSStatInit(); //开启统计任务
	OS_ENTER_CRITICAL(); //进入临界区(关闭中断)
	OSTaskCreate(_UCOS_LED_G_TASK, 0, &UCOS_LED_G_TASK_STK[UCOS_TASK_LED_G_STK_SIZE - 1], UCOS_TASK_LED_G_PRIO);
	OSTaskCreate(_UCOS_LED_R_TASK, 0, &UCOS_LED_R_TASK_STK[UCOS_TASK_LED_R_STK_SIZE - 1], UCOS_TASK_LED_R_PRIO);
	OSTaskCreate(_MOTOR_TASK, 0, &UCOS_MOTOR_TASK_STK[UCOS_TASK_MOTOR_STK_SIZE - 1], UCOS_TASK_MOTOR_PRIO);
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

void _UCOS_LED_R_TASK(void *pdata)
{
	while(1)
	{
		LED_R_control(0);
		malls(1);
		LED_R_control(1);
		malls(1);
	}
}

void _MOTOR_TASK(void *pdata)
{
	while(1)
	{
		mallms(10);
		
	}
}


