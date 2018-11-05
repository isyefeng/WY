#include "bsp_AdvanceTim.h" 
#include "Systick_Dirve.h"
#include "LED.h"
#include "Includes.h"

static void MOTO_Drive_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(MOTO1_PIR_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(MOTO2_PIR_GPIO_CLK, ENABLE);


	GPIO_InitStruct.GPIO_Pin =  MOTO1_PIR_GPIO_PIN;
  	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(MOTO1_PIR_GPIO_PORT, &GPIO_InitStruct);
	 
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Pin =  MOTO2_PIR_GPIO_PIN;
	 GPIO_Init(MOTO2_PIR_GPIO_PORT, &GPIO_InitStruct);

}

static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���PWM1ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

    // ���PWM2ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(ADVANCE_TIM8_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM8_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM8_CH1_PORT, &GPIO_InitStructure);


}


///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)


/**********************���1*********************************/
void Motor_tim_config(uint16_t offset)
{
	// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=offset;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
}

static void ADVANCE_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ����ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	// ʹ�ܼ�����
	TIM_Cmd(ADVANCE_TIM, DISABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_ITConfig(ADVANCE_TIM, TIM_IT_Update, ENABLE);
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

static void Moto_pwm_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}





/***********************���2******************************/
void Motor_tim8_config(uint16_t offset)
{
	// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM8_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=offset;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM8, &TIM_TimeBaseStructure);
}

static void ADVANCE_TIM8_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM8_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(ADVANCE_TIM8, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ����ռ�ձȴ�С
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// ���ͨ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// ���ͨ�����е�ƽ��������
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(ADVANCE_TIM8, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM8, TIM_OCPreload_Enable);
	// ʹ�ܼ�����
	TIM_Cmd(ADVANCE_TIM8, DISABLE);	
	// �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
	TIM_ITConfig(ADVANCE_TIM8, TIM_IT_Update, ENABLE);
	TIM_CtrlPWMOutputs(ADVANCE_TIM8, ENABLE);
}

static void Moto_pwm2_nvic_config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM8_UP_IRQn ;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}









void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_GPIO_Config();
	MOTO_Drive_GPIO_Config();
	Moto_pwm_nvic_config();
	ADVANCE_TIM_Mode_Config();	

	/*���2tim��������*/
	Moto_pwm2_nvic_config();
	ADVANCE_TIM8_Mode_Config();
}

void TIM1_UP_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(ADVANCE_TIM, TIM_IT_Update) != RESET)
	{		
		TIM_Cmd(ADVANCE_TIM, DISABLE);	
		TIM_ClearITPendingBit( ADVANCE_TIM, TIM_IT_Update);
	}	 
}

void	TIM8_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(ADVANCE_TIM8, TIM_IT_Update) != RESET)
	{		
		TIM_Cmd(ADVANCE_TIM8, DISABLE);	
		TIM_ClearITPendingBit( ADVANCE_TIM8, TIM_IT_Update);
	}	
}

/*���õ��1��ת�Ƕ�*/
void Moto1_rotate_angle(uint16_t offset)
{
	uint16_t buf = 0;
	
	buf = offset*100/45;
	Motor_tim_config(buf);
	TIM_Cmd(ADVANCE_TIM, ENABLE);
}

/*���õ��2��ת�Ƕ�*/
void Moto2_rotate_angle(uint16_t offset)
{
	uint16_t buf = 0;
	
	buf = offset*100/45;
	Motor_tim8_config(buf);
	TIM_Cmd(ADVANCE_TIM8, ENABLE);
}

uint8_t Motor_switch(void)
{
	static uint8_t dat = 0;
	static uint8_t temp = 2;
//	static uint8_t offset = 0;
	
	if(GPIO_ReadInputDataBit(GPIO_KEY2_RORT,GPIO_KEY2_PIN))
	{
		if(temp == 2)
		{
			dat = 1;
			temp--;
			Moto1_rotate_angle(90);
			Moto2_rotate_angle(90);
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


uint8_t PWM_TUGO(void)
{
	static uint8_t dat = 0;
	static uint8_t temp = 2;
	
	if(GPIO_ReadInputDataBit(GPIO_KEY1_RORT,GPIO_KEY1_PIN))
	{
		if(temp == 2)
		{
			dat = 1;
			digitalToggle(GPIOB, GPIO_Pin_5);
			digitalToggle(GPIOB, GPIO_Pin_12);
			digitalToggle(GPIOB, GPIO_Pin_6);
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


/*********************************************END OF FILE**********************/
