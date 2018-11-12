/*
************************************************************************************************
主要的包含文件

文 件: INCLUDES.C ucos包含文件
作 者: Jean J. Labrosse
************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"

#include <stm32f10x.h>	
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
#include "string.h"
#include "delay.h"
#include "bsp_AdvanceTim.h" 
#include "DS18B20_Drive.h"
#include "os_task_create.h"
#include "app_TJ_modole.h"
#include "BT_mod.h"
#include "OSP_fifo.h"


#define	TURE	1
#define	FLASE	0

#define	malls(a)  	OSTimeDlyHMSM(0, 0, a, 0);
#define	mallms(a)	OSTimeDlyHMSM(0, 0, 0, a);

extern float	 g_temperture_num;	//温度
extern float	g_moto1_angle;		//电机1角度
extern float	g_moto2_angle;		//电机2角度
extern uint8_t g_temperture_set;	//设置温度		(常温~99);
extern uint8_t g_flow_set;			//设置流量大小   (0~9)
extern uint8_t 	 g_moto_open;		//开始标志
extern unsigned char _usart_fifo_buf[];

#endif































