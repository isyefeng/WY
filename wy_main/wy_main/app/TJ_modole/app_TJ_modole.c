#include "Includes.h"

#define	MOTO_OFF 	0
#define	MOTO_ON 	1

#define	MOTO_ANGLE_ASTIRCT_PRIO		10
#define	MOTO_ANGLE_ASTIRCT_SIZE			128
OS_STK	MOTO_ANGLE_ASTIRCT_TASK_STK[MOTO_ANGLE_ASTIRCT_SIZE];

uint8_t Get_temperture_set(void)
{
	return g_temperture_set;
}

/*����Ƕ���������*/
void _MOTO_ANGLE_ASTIRCT_TASK(void *pdata)
{
	while(1)
	{
		mallms(10);
		
	}
}

void temperture_set(uint8_t num)
{
	int offset = 0;

	while(1)
	{
		mallms(10);
		if(g_moto2_angle <= 0)					//�жϵ���Ƕ��Ƿ���ȷ
		{
			MOTOR2_OFF();
			continue;
		}
		if(g_moto1_angle >= num)			//�жϵ���Ƕ��Ƿ���ȷ
		{
			MOTOR1_OFF();
			continue;
		}
		offset = g_temperture_num - g_temperture_set;
		if(offset >10)
		{
			Moto2_rotate_angle(-2);
			Moto1_rotate_angle(2);
			mallms(500);
		}
		if(offset >0&&offset<10)
		{
			Moto2_rotate_angle(-1);
			Moto1_rotate_angle(1);
			mallms(500);
		}
		if(offset == 0)
		{
			MOTOR2_OFF();
			MOTOR1_OFF();
		}
		if(offset < 0)
		{
			Moto1_rotate_angle(-1);
			Moto2_rotate_angle(1);
			mallms(500);
		}
	}
}

/*�����������*/
void _MOTOR_TASK(void *pdata)
{
	float moto2_angle = 0;

	OSTaskCreate(     _MOTO_ANGLE_ASTIRCT_TASK,
					0, 
					&MOTO_ANGLE_ASTIRCT_TASK_STK[MOTO_ANGLE_ASTIRCT_SIZE - 1], 
					MOTO_ANGLE_ASTIRCT_PRIO);
	while(1)
	{
		mallms(10);
//		Motor_switch();
		if(g_moto_open == MOTO_OFF)
			continue;
		moto2_angle = ((float)g_flow_set/10)*90;
		Moto2_rotate_angle(moto2_angle);
		while(1)
		{
			mallms(10);
			if( g_temperture_num>g_temperture_set)
			{
				Moto2_rotate_angle(-10);
				Moto1_rotate_angle(10);
				temperture_set(moto2_angle);
			}
		}

	}
}


