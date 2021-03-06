/*-------------------------------------------------------------------------
    
-------------------------------------------------------------------------*/
#include "key_drive.h"
#include "LED.h"
#include "bsp_AdvanceTim.h"

void Key1_Init(void)
{
	GPIO_InitTypeDef KEY1_Initstruct;
	
	RCC_APB2PeriphClockCmd(GPIO_KEY1_RCC, ENABLE);
	
	KEY1_Initstruct.GPIO_Pin = GPIO_KEY1_PIN;
	KEY1_Initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_KEY1_RORT,&KEY1_Initstruct);	
}

void Key2_Init(void)
{
	GPIO_InitTypeDef KEY2_Initstruct;
	
	RCC_APB2PeriphClockCmd(GPIO_KEY2_RCC, ENABLE);
	
	KEY2_Initstruct.GPIO_Pin = GPIO_KEY2_PIN;
	KEY2_Initstruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_KEY2_RORT,&KEY2_Initstruct);	
}

uint8_t Key1_Read(void)
{
	static uint8_t dat = 0;
	static uint8_t temp = 2;
	
	if(GPIO_ReadInputDataBit(GPIO_KEY1_RORT,GPIO_KEY1_PIN))
	{
		if(temp == 2)
		{
			dat = 1;
			temp--;
		}
		else if(temp == 1)
		{
			temp = 1;
		} 
	}
	else
	{
		temp = 2;
		dat = 0;
	}
	
	return dat;
}

